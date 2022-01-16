#include "commandqueue.h"
#include "commandlist.h"
#include "dx12_backend.h"
#include <vector>

namespace fccs {
	namespace rhi {

		CommandQueue::CommandQueue(ID3D12Device* pDevice, CommandQueueType type) {
			m_device = pDevice;
			m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			if (type == CommandQueueType::Graphics) {
				queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			}
			else if (type == CommandQueueType::Compute) {
				queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
			}
			else {
				queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
			}
			m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_queue));
			m_event.Attach(CreateEventW(0, 0, 0, 0));
			submitValue = 0;
			addCommandAllocatorRefCount();
		}

		CommandQueue::~CommandQueue() {
			submitValue++;
			m_queue->Signal(m_fence.Get(), submitValue);
			m_fence->SetEventOnCompletion(submitValue, m_event.Get());
			WaitForSingleObjectEx(m_event.Get(), INFINITE, FALSE);

			releaseCommandAllocatorRef();
		}

		uint64_t CommandQueue::getCompletedValue() {
			return m_fence->GetCompletedValue();
		}

		void CommandQueue::ExecuteCommandLists(uint32_t NumCommandLists, ICommandList* const* ppCommandLists) {
			std::vector<ID3D12CommandList*> m_CommandListsToExecute(NumCommandLists);
			for (size_t i = 0; i < NumCommandLists; i++)
			{
				m_CommandListsToExecute[i] = (ID3D12CommandList*)(static_cast<CommandList*>(ppCommandLists[i])->GetNativePtr());
			}
			
			m_queue->ExecuteCommandLists(NumCommandLists, m_CommandListsToExecute.data());
			submitValue++;
			m_queue->Signal(m_fence.Get(), submitValue);

			for (size_t i = 0; i < NumCommandLists; i++)
			{
				updateBusyAllocator(static_cast<CommandList*>(ppCommandLists[i])->m_Alloc.Get(), m_fence.Get(), submitValue);
			}
		}

	}
}