#include "dx12_backend.h"
#include "format.h"
#include <Windows.h>
#include <wrl/client.h>
#include <mutex>
#include <vector>
namespace fccs {
	typedef HRESULT(WINAPI* CreateFactory)(UINT, REFIID, _COM_Outptr_ void**);

	void createD3D12Device(ID3D12Device** ppDevice) {
		*ppDevice = nullptr;
		static auto mod = LoadLibraryW(L"d3d12.dll");
		auto createDevice = (PFN_D3D12_CREATE_DEVICE)GetProcAddress(mod, "D3D12CreateDevice");
		Microsoft::WRL::ComPtr<ID3D12Device> device;
		createDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
		*ppDevice = device.Detach();
	}

	void createDXGIFactory(IDXGIFactory **ppFactory) {
		*ppFactory = nullptr;
		static auto mod = LoadLibraryW(L"dxgi.dll");
		auto createFactory = (CreateFactory)GetProcAddress(mod, "CreateDXGIFactory2");
		Microsoft::WRL::ComPtr<IDXGIFactory> factory;
#ifdef _DEBUG
		createFactory(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&factory));
#else
		createFactory(0, IID_PPV_ARGS(&factory));
#endif
		*ppFactory = factory.Detach();
	}

	struct CmdAllocator {
		ID3D12Device* device = nullptr;
		ID3D12Fence* fence = nullptr;
		uint64_t submitValue = 0;
		rhi::CommandQueueType type = rhi::CommandQueueType::Graphics;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	};

	struct CmdAllocatorPool {
		uint32_t refCount = 0;
		std::mutex mtx;
		std::vector<CmdAllocator> canUseAllocator;
		std::vector<CmdAllocator> busyAllocator;

		ID3D12CommandAllocator* getCanUseAllocatorByDeviceAndType(ID3D12Device* pDevice, rhi::CommandQueueType type) {
			//if
			CmdAllocator tempAlloc = {};
			
			std::lock_guard<std::mutex> lck(mtx);
			for (auto iter = canUseAllocator.begin(); iter != canUseAllocator.end(); ++iter) {
				if (iter->device == pDevice && iter->type == type) {
					tempAlloc = *iter;
					tempAlloc.cmdAllocator->Reset();
					canUseAllocator.erase(iter);
					break;
				}
			}

			if (!tempAlloc.device) {
				if (type == rhi::CommandQueueType::Graphics) {
					pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&tempAlloc.cmdAllocator));
				}
				else if (type == rhi::CommandQueueType::Compute) {
					pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COMPUTE, IID_PPV_ARGS(&tempAlloc.cmdAllocator));
				}
				else {
					pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COPY, IID_PPV_ARGS(&tempAlloc.cmdAllocator));
				}
				tempAlloc.device = pDevice;
				tempAlloc.type = type;
			}

			busyAllocator.emplace_back(tempAlloc);
			return busyAllocator.back().cmdAllocator.Get();
		}

		void updateBusyAllocator(ID3D12CommandAllocator* pAllocator, ID3D12Fence* pFence, uint64_t submitValue) {
			std::lock_guard<std::mutex> lck(mtx);
		
			auto iter = busyAllocator.begin();
			while (iter != busyAllocator.end()) {
				if (iter->cmdAllocator.Get() == pAllocator) {
					iter->fence = pFence;
					iter->submitValue = submitValue;
				}
				if (iter->fence && iter->submitValue != 0 && iter->fence->GetCompletedValue() >= iter->submitValue) {
					canUseAllocator.emplace_back(*iter);
					iter = busyAllocator.erase(iter);
				}
				else {
					++iter;
				}
			}
		}
	};

	static CmdAllocatorPool cmd_Pool;

	ID3D12CommandAllocator* getCommandAllocatorInPool(ID3D12Device* pDevice, rhi::CommandQueueType type) {
		return cmd_Pool.getCanUseAllocatorByDeviceAndType(pDevice, type);
	}

	void updateBusyAllocator(ID3D12CommandAllocator* pAllocator, ID3D12Fence* pFence, uint64_t submitValue) {
		cmd_Pool.updateBusyAllocator(pAllocator, pFence, submitValue);
	}

	void addCommandAllocatorRefCount() {
		std::lock_guard<std::mutex> lck(cmd_Pool.mtx);
		cmd_Pool.refCount++;
	}

	void releaseCommandAllocatorRef() {
		std::lock_guard<std::mutex> lck(cmd_Pool.mtx);
		cmd_Pool.refCount--;
		if (cmd_Pool.refCount == 0) {
			cmd_Pool.canUseAllocator.clear();
			cmd_Pool.busyAllocator.clear();
		}
	}

	namespace rhi {
		D3D12_RESOURCE_DESC convertTextureDesc(const TextureDesc& d) {

			const auto& formatMapping = getDxgiFormatMapping(d.format);

			D3D12_RESOURCE_DESC desc = {};
			desc.Width = d.width;
			desc.Height = d.height;
			desc.MipLevels = UINT16(d.mipLevels);
			desc.Format = d.isTypeless ? formatMapping.resourceFormat : formatMapping.rtvFormat;
			desc.SampleDesc.Count = d.sampleCount;
			desc.SampleDesc.Quality = d.sampleQuality;

			switch (d.dimension)
			{
			case TextureDimension::Texture1D:
			case TextureDimension::Texture1DArray:
				desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
				desc.DepthOrArraySize = UINT16(d.arraySize);
				break;
			case TextureDimension::Texture2D:
			case TextureDimension::Texture2DArray:
			case TextureDimension::TextureCube:
			case TextureDimension::TextureCubeArray:
			case TextureDimension::Texture2DMS:
			case TextureDimension::Texture2DMSArray:
				desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
				desc.DepthOrArraySize = UINT16(d.arraySize);
				break;
			case TextureDimension::Texture3D:
				desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
				desc.DepthOrArraySize = UINT16(d.depth);
				break;
			//case TextureDimension::Unknown:
			//default:
			//	//todo error
			//	
			//	break;
			}

			if (d.isRenderTarget)
			{
				if (d.format == Format::D16 || d.format == Format::D24S8 || d.format == Format::X24G8_UINT || d.format == Format::D32 || d.format == Format::D32S8 || d.format == Format::X32G8_UINT) {
					desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
				}
				else {
					desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
				}
			}

			if (d.isUAV)
				desc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

			return desc;
		}

		D3D12_RESOURCE_STATES convertResourceStates(ResourceStates stateBits)
		{
			if (stateBits == ResourceStates::Common)
				return D3D12_RESOURCE_STATE_COMMON;

			D3D12_RESOURCE_STATES result = D3D12_RESOURCE_STATE_COMMON; // also 0

			if ((stateBits & ResourceStates::ConstantBuffer) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
			if ((stateBits & ResourceStates::VertexBuffer) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
			if ((stateBits & ResourceStates::IndexBuffer) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_INDEX_BUFFER;
			if ((stateBits & ResourceStates::IndirectArgument) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;
			if ((stateBits & ResourceStates::ShaderResource) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
			if ((stateBits & ResourceStates::UnorderedAccess) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
			if ((stateBits & ResourceStates::RenderTarget) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_RENDER_TARGET;
			if ((stateBits & ResourceStates::DepthWrite) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
			if ((stateBits & ResourceStates::DepthRead) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_DEPTH_READ;
			if ((stateBits & ResourceStates::StreamOut) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_STREAM_OUT;
			if ((stateBits & ResourceStates::CopyDest) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_COPY_DEST;
			if ((stateBits & ResourceStates::CopySource) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_COPY_SOURCE;
			if ((stateBits & ResourceStates::ResolveDest) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_RESOLVE_DEST;
			if ((stateBits & ResourceStates::ResolveSource) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_RESOLVE_SOURCE;
			if ((stateBits & ResourceStates::Present) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_PRESENT;
			if ((stateBits & ResourceStates::AccelStructRead) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
			if ((stateBits & ResourceStates::AccelStructWrite) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
			if ((stateBits & ResourceStates::AccelStructBuildInput) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
			if ((stateBits & ResourceStates::AccelStructBuildBlas) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE;
			if ((stateBits & ResourceStates::ShadingRateSurface) != ResourceStates::Unknown) result |= D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE;

			return result;
		}
	}

}