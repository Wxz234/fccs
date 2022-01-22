#pragma once
#include <fccs/rhi.h>
#include <wrl/client.h>
#include <unordered_map>
#include "texture.h"
namespace fccs {
	namespace rhi {


		struct textureState {

		};

		class CommandList : public ICommandList {
		public:
			CommandList(ID3D12Device* pDevice, CommandQueueType type);
			~CommandList();
			void* GetNativePtr() const noexcept {
				return m_mainLists.Get();
			}
			void ChangeTextureState(ITexture* pTexure, ResourceStates state);
			void Open();
			void Close();
			
			Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_Alloc;
			Microsoft::WRL::ComPtr<ID3D12Device> m_device;

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> m_mainLists;

			std::unordered_map<ITexture*, ResourceStates> m_texture_state;
			CommandQueueType m_Desc;
		};

	}
}