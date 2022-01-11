#pragma once
#include <fccs/fccs.h>
#include <list>
#include "dx12_backend.h"
namespace fccs {
	namespace rhi {
		class CommandList : public ICommandList {
		public:
			CommandList(ID3D12Device* pDevice, QueueRef* pQueue, CommandListParameters parameters);
			void* getNativePtr() const noexcept;
		};
		Microsoft::WRL::ComPtr<ID3D12Device> m_device;
		QueueRef* m_queue;
		CommandListParameters m_para;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_MainList;
		std::list<std::shared_ptr<InternalCommandList>> m_CommandListPool;
		std::shared_ptr<InternalCommandList> m_ActiveCommandList;
	}
}