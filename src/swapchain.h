#pragma once
#include <fccs/fccs.h>
#include <wrl.h>
#include <dxgi1_6.h>
#include <vector>
#define FCCS_SWAPCHAIN_NUM 3
namespace fccs {


	namespace window {
		class SwapChain : public ISwapChain {
		public:
			SwapChain(ID3D12CommandQueue* pQueue, HWND hwnd, uint32_t width, uint32_t height);
			D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView(uint32_t n) const noexcept;
			void Present(uint32_t sync);
			Format GetRenderTargetFormat() const noexcept;
			~SwapChain();
			Microsoft::WRL::ComPtr < ID3D12CommandQueue> m_queue;
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