#pragma once
#include <fccs/rhi.h>
#include <wrl/client.h>
namespace fccs {
	namespace rhi {
		class Texture : public ITexture {
		public:
			Texture(ID3D12Resource* resource, const TextureDesc& desc) :m_res(resource),m_desc(desc) {
				
			}
			Microsoft::WRL::ComPtr<ID3D12Resource> m_res;
			TextureDesc m_desc;
		};
	}
}
