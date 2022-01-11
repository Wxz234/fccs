#pragma once
#include <fccs/fccs.h>
#include <list>
#include "dx12_backend.h"
namespace fccs {
	namespace rhi {
		class CommandList : public ICommandList {
		public:
			CommandList(ID3D12Device* device, QueueRef* queue, const CommandListParameters& params);
			std::shared_ptr<InternalCommandList> CommandList::createInternalCommandList()const;
			ID3D12CommandList* getD3D12CommandList();
			void executed(QueueRef* pQueue);
			//Microsoft::WRL::ComPtr<ID3D12CommandList> m_MainList;
			void open();
			void close();

			void* CommandList::getNativePtr() const noexcept;

			Microsoft::WRL::ComPtr<ID3D12Device> m_device;
			CommandListParameters m_Desc;
			std::shared_ptr<InternalCommandList> m_ActiveCommandList;
			std::list<std::shared_ptr<InternalCommandList>> m_CommandListPool;
			QueueRef* m_Queue;
		};

	}
}