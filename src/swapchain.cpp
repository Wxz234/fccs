#include "swapchain.h"
#include "dx12_backend.h"
namespace fccs {
	namespace window {
		SwapChain::SwapChain(ID3D12CommandQueue* pQueue, HWND hwnd, uint32_t width, uint32_t height) {
			m_queue = pQueue;

			Microsoft::WRL::ComPtr<IDXGIFactory> factory;
			createDXGIFactory(&factory);

			DXGI_SWAP_CHAIN_DESC1 _desc = {};
			_desc.BufferCount = FCCS_SWAPCHAIN_NUM;
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
			factory4->CreateSwapChainForHwnd(m_queue.Get(), hwnd, &_desc, &fsSwapChainDesc, nullptr, &swapChain);
			factory4->MakeWindowAssociation(hwnd, DXGI_MWA_NO_WINDOW_CHANGES | DXGI_MWA_NO_ALT_ENTER);
			swapChain.As(&m_swapchain);
			m_event.Attach(CreateEvent(nullptr, FALSE, FALSE, nullptr));
			m_frameIndex = m_swapchain->GetCurrentBackBufferIndex();
			m_fenceValues.resize(_desc.BufferCount, 0);

			Microsoft::WRL::ComPtr<ID3D12Device8> _device;
			m_queue->GetDevice(IID_PPV_ARGS(&_device));
			_device->CreateFence(m_fenceValues[m_frameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
			++m_fenceValues[m_frameIndex];

			D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc = {};
			rtvDescriptorHeapDesc.NumDescriptors = FCCS_SWAPCHAIN_NUM;
			rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			_device->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(m_rtvDescriptorHeap.ReleaseAndGetAddressOf()));

			for (uint32_t n = 0; n < FCCS_SWAPCHAIN_NUM; ++n)
			{
				m_swapchain->GetBuffer(n, IID_PPV_ARGS(m_Resource[n].GetAddressOf()));

				m_rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
				D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptor = m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
				rtvDescriptor.ptr = SIZE_T(INT64(rtvDescriptor.ptr) + INT64(n) * INT64(m_rtvDescriptorSize));
				_device->CreateRenderTargetView(m_Resource[n].Get(), nullptr, rtvDescriptor);
			}
		}
		uint32_t SwapChain::GetCurrentBackBufferIndex()  const noexcept {
			return m_swapchain->GetCurrentBackBufferIndex();
		}
		ID3D12Resource* SwapChain::GetNativeResourcePtr(uint32_t n) const noexcept {
			return m_Resource[n].Get();
		}
		D3D12_CPU_DESCRIPTOR_HANDLE SwapChain::GetRenderTargetView(uint32_t n) const noexcept {
			D3D12_CPU_DESCRIPTOR_HANDLE rtvDescriptor = m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
			rtvDescriptor.ptr = SIZE_T(INT64(rtvDescriptor.ptr) + INT64(n) * INT64(m_rtvDescriptorSize));
			return rtvDescriptor;
		}

		void SwapChain::Present(uint32_t sync) {
			m_swapchain->Present(sync, 0);
			const uint64_t currentFenceValue = m_fenceValues[m_frameIndex];
			m_queue->Signal(m_fence.Get(), currentFenceValue);
			m_frameIndex = m_swapchain->GetCurrentBackBufferIndex();
			if (m_fence->GetCompletedValue() < m_fenceValues[m_frameIndex]) {
				m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_event.Get());
				WaitForSingleObjectEx(m_event.Get(), INFINITE, FALSE);
			}
			m_fenceValues[m_frameIndex] = currentFenceValue + 1;
		}

		Format SwapChain::GetRenderTargetFormat() const noexcept {
			return Format::RGBA8_UNORM;
		}

		SwapChain::~SwapChain() {
			m_queue->Signal(m_fence.Get(), m_fenceValues[m_frameIndex]);
			m_fence->SetEventOnCompletion(m_fenceValues[m_frameIndex], m_event.Get());
			WaitForSingleObjectEx(m_event.Get(), INFINITE, FALSE);
			++m_fenceValues[m_frameIndex];
		}

		FCCS_API SwapChainHandle CreateSwapChain(const SwapChainDesc& desc, rhi::ICommandQueue* pQueue) {
			return SwapChainHandle(new SwapChain((ID3D12CommandQueue*)pQueue->GetNativePtr(), desc.hwnd, desc.width, desc.height));
		}
	}
}