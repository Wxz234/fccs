#pragma once
#include <fccs/fccs.h>
#include <wrl/client.h>
#include <dxgi1_6.h>
namespace fccs {
	namespace window {
		class SwapChain : public ISwapChain {
		public:
			SwapChain(ID3D12CommandQueue* pQueue, HWND hwnd, uint32_t width, uint32_t height);
			void Present(uint32_t sync) {}

			ID3D12CommandQueue* m_queue;
			Microsoft::WRL::ComPtr<IDXGISwapChain4> m_swapchain;

		};
	}
}