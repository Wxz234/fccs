#pragma once
#define FCCS_API

#include <cstdint>
#include <cstddef>
#include <string>
#include <d3d12.h>
#include <windef.h>
#include "core.h"


namespace fccs {

	namespace rhi {

		class IRHIObject : public IResource {
		public:
			virtual void* GetNativePtr() const noexcept = 0;
		};

		enum class ResourceStates : uint32_t {
			Unknown					= 0,
			Common					= 0x00000001,
			ConstantBuffer			= 0x00000002,
			VertexBuffer			= 0x00000004,
			IndexBuffer				= 0x00000008,
			IndirectArgument		= 0x00000010,
			ShaderResource			= 0x00000020,
			UnorderedAccess			= 0x00000040,
			RenderTarget			= 0x00000080,
			DepthWrite				= 0x00000100,
			DepthRead				= 0x00000200,
			StreamOut				= 0x00000400,
			CopyDest				= 0x00000800,
			CopySource				= 0x00001000,
			ResolveDest				= 0x00002000,
			ResolveSource			= 0x00004000,
			Present					= 0x00008000,
			AccelStructRead			= 0x00010000,
			AccelStructWrite		= 0x00020000,
			AccelStructBuildInput	= 0x00040000,
			AccelStructBuildBlas	= 0x00080000,
			ShadingRateSurface		= 0x00100000,
		};
		FCCS_BITMASK_OPS(ResourceStates)

		enum class CommandQueueType : uint8_t
		{
			Graphics = 0,
			Compute,
			Copy,
		};

		class ICommandList : public IRHIObject {
		public:
			virtual void Open() = 0;
			virtual void Close() = 0;
		};

		class ICommandQueue : public IRHIObject {
		public:
			virtual void ExecuteCommandLists(uint32_t NumCommandLists, ICommandList* const* ppCommandLists) = 0;
		};

		struct DeviceDesc
		{
			ID3D12Device* pDevice = nullptr;
		};

		class IDevice : public IRHIObject {
		public:
			virtual ICommandList* CreateCommandList(CommandQueueType type) = 0;
			virtual ICommandQueue* CreateCommandQueue(CommandQueueType type) = 0;
		};

		FCCS_API IDevice* CreateDeivce(const DeviceDesc& desc = DeviceDesc());

		class ISwapChain : public IResource {
		public:
			virtual uint32_t GetCurrentBackBufferIndex() const noexcept = 0;
			virtual ID3D12Resource* GetNativeResourcePtr(uint32_t n) const noexcept = 0;
			virtual D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView(uint32_t n) const noexcept = 0;
			virtual void Present(uint32_t sync) = 0;
		};

		struct SwapChainDesc
		{
			uint32_t width;
			uint32_t height;
			HWND hwnd;
		};
		FCCS_API ISwapChain* CreateSwapChain(ICommandQueue* pQueue, const SwapChainDesc& desc);
	}

	namespace framegraph {

		class Pass {
		public:

		};

		class IFrameGraph {
		public:
			IFrameGraph() {}

			void AddPass(const Pass& pass);
			void Compile();
			void Execute();
		private:
		};

	}

	namespace window {

		struct WindowDesc
		{
			uint32_t width;
			uint32_t height;
			std::wstring title;
		};

		class IWindow : public IResource {
		public:
			virtual HWND GetHWND() const noexcept = 0;
			virtual void OpenWindow() = 0;
		private:
		};
		FCCS_API IWindow* CreateFCCSWindow(const WindowDesc& desc);
	}
}
