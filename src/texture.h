#pragma once
#include <fccs/rhi.h>
#include <wrl/client.h>
#include <vector>
namespace fccs {
	namespace rhi {

		class TextureInterface : public ITexture {
		public:
			virtual ResourceStates GetInitialState() = 0;
		};

		class Texture : public TextureInterface {
		public:
			Texture(ID3D12Resource* resource, const TextureDesc& desc) :m_res(resource),m_desc(desc) {}
			~Texture() {}
			ResourceStates GetInitialState() {
				return m_desc.initialState;
			}
			void* GetNativePtr() const noexcept {
				return m_res.Get();
			}
			Microsoft::WRL::ComPtr<ID3D12Device> m_device;
			Microsoft::WRL::ComPtr<ID3D12Resource> m_res;

			TextureDesc m_desc;
		};


		class SwapChainTexture : public TextureInterface {
		public:
			SwapChainTexture(ID3D12Resource* resource) : m_res(resource), initialState(ResourceStates::Present) {}

			ResourceStates GetInitialState() {
				return initialState;
			}
			void* GetNativePtr() const noexcept {
				return m_res.Get();
			}
			Microsoft::WRL::ComPtr<ID3D12Resource> m_res;
			ResourceStates initialState;
		};


	}


}
