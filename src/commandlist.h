#pragma once
#include <fccs/fccs.h>

#include <wrl/client.h>
namespace fccs {
	namespace rhi {
		class CommandList : public ICommandList {
		public:
			CommandList(ID3D12Device* pDevice, CommandQueueType type);
			void* GetNativePtr() const noexcept {
				return m_mainLists.Get();
			}

			void Open();
			void Close();
			
			ID3D12CommandAllocator* m_Alloc;
			ID3D12Device* m_device;

			Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> m_mainLists;

			CommandQueueType m_Desc;
		};

	}
}