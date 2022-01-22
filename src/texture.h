#pragma once
#include <fccs/rhi.h>
#include <wrl/client.h>
#include <vector>
namespace fccs {
	namespace rhi {
		class Texture : public ITexture {
		public:
			Texture(ID3D12Resource* resource, const TextureDesc& desc) :m_res(resource),m_desc(desc) {
				
			}
			~Texture() {
				for (auto heap : m_srv) {
					heap->Release();
				}
				for (auto heap : m_uav) {
					heap->Release();
				}
				for (auto heap : m_rtv) {
					heap->Release();
				}
				for (auto heap : m_dsv) {
					heap->Release();
				}
			}
			void* GetNativePtr() const noexcept {
				return m_res.Get();
			}
			Microsoft::WRL::ComPtr<ID3D12Device> m_device;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_res;
			std::vector<ID3D12DescriptorHeap*> m_srv;
			std::vector<ID3D12DescriptorHeap*> m_uav;
			std::vector<ID3D12DescriptorHeap*> m_rtv;
			std::vector<ID3D12DescriptorHeap*> m_dsv;
			TextureDesc m_desc;
		};


		class SwapChainTexture : public ITexture {
		public:
			SwapChainTexture(ID3D12Resource* resource) : m_res(resource) {}

			void* GetNativePtr() const noexcept {
				return m_res.Get();
			}

			Microsoft::WRL::ComPtr<ID3D12Resource> m_res;

		};
	}


}
