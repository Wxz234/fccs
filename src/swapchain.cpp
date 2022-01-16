#include "swapchain.h"
#include "dx12_backend.h"
namespace fccs {
	namespace window {
		SwapChain::SwapChain(ID3D12CommandQueue* pQueue, HWND hwnd, uint32_t width, uint32_t height) {
			m_queue = pQueue;

			Microsoft::WRL::ComPtr<IDXGIFactory> factory;
			createDXGIFactory(&factory);

			DXGI_SWAP_CHAIN_DESC1 _desc = {};
			_desc.BufferCount = 3;
			_desc.Width = width;
			_desc.Height = height;
			_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			_desc.SampleDesc.Count = 1;
			_desc.SampleDesc.Quality = 0;
			_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			_desc.Flags = 0;
			_desc.Scaling = DXGI_SCALING_STRETCH;
			_desc.Stereo = FALSE;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
			fsSwapChainDesc.Windowed = TRUE;

			Microsoft::WRL::ComPtr<IDXGIFactory4> factory4;
			Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
			factory.As(&factory4);
			factory4->CreateSwapChainForHwnd(m_queue, hwnd, &_desc, &fsSwapChainDesc, nullptr, &swapChain);
			factory4->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
			swapChain.As(&m_swapchain);
			
		}
	}
}