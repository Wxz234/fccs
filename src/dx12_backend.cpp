#include "dx12_backend.h"

#include <wrl/client.h>
#include <vector>
#include <mutex> 
namespace fccs {
	void createD3D12Device(ID3D12Device** ppDevice) {
		*ppDevice = nullptr;
		static auto mod = LoadLibraryW(L"d3d12.dll");
		auto createDevice = (PFN_D3D12_CREATE_DEVICE)GetProcAddress(mod, "D3D12CreateDevice");
		Microsoft::WRL::ComPtr<ID3D12Device> device;
		createDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device));
		*ppDevice = device.Detach();
	}

	struct CmdAllocator {
		ID3D12Device* device = nullptr;
		ID3D12Fence* fence = nullptr;
		uint64_t submitValue = 0;
		rhi::CommandQueueType type = rhi::CommandQueueType::Graphics;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	};

	struct CmdAllocatorPool {

		std::mutex mtx;
		std::vector<CmdAllocator> canUseAllocator;
		std::vector<CmdAllocator> busyAllocator;

		ID3D12CommandAllocator* getCanUseAllocatorByDeviceAndType(ID3D12Device* pDevice, rhi::CommandQueueType type) {
			//if
			CmdAllocator tempAlloc = {};
			{
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
			}

			return tempAlloc.cmdAllocator.Get();
		}

		void updateBusyAllocator(ID3D12CommandAllocator* pAllocator, ID3D12Fence* pFence, uint64_t submitValue) {
			{
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
		}
	};

	CmdAllocatorPool cmd_Pool;

	ID3D12CommandAllocator* getCommandAllocatorInPool(ID3D12Device* pDevice, rhi::CommandQueueType type) {
		return cmd_Pool.getCanUseAllocatorByDeviceAndType(pDevice, type);
	}

	void updateBusyAllocator(ID3D12CommandAllocator* pAllocator, ID3D12Fence* pFence, uint64_t submitValue) {
		cmd_Pool.updateBusyAllocator(pAllocator, pFence, submitValue);
	}

}