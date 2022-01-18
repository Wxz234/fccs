#pragma once
#include <fccs/rhi.h>
#include <wrl.h>
#include <vector>
#define FCCS_SWAPCHAIN_NUM 3
namespace fccs {


	namespace rhi {
		class SwapChain : public ISwapChain {
		public:
			SwapChain(ID3D12CommandQueue* pQueue, HWND hwnd, uint32_t width, uint32_t height);
			uint32_t GetCurrentBackBufferIndex() const noexcept;
			ID3D12Resource* GetNativeResourcePtr(uint32_t n) const noexcept;
			D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView(uint32_t n) const noexcept;
			void Present(uint32_t sync);
			~SwapChain();
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_queue;
			Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapchain;

			//sync
			Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
			Microsoft::WRL::Wrappers::Event m_event;
			std::vector<uint64_t> m_fenceValues;
			uint32_t m_frameIndex;
			//
			Microsoft::WRL::ComPtr<ID3D12Resource> m_Resource[FCCS_SWAPCHAIN_NUM];
			Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap;
			uint32_t m_rtvDescriptorSize;
		};
	}
}