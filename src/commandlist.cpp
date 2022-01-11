#include "commandlist.h"

namespace fccs {
	namespace rhi {
        CommandList::CommandList(ID3D12Device* device, QueueRef* queue, const CommandListParameters& params) {
            m_device = device;
            m_Queue = queue;
            m_Desc = params;
        }

		ID3D12CommandList* CommandList::getD3D12CommandList() {
			return m_ActiveCommandList->commandList.Get();
		}

		void CommandList::executed(QueueRef* pQueue) {
			m_ActiveCommandList->lastSubmittedInstance = pQueue->lastSubmittedInstance;
			m_CommandListPool.push_back(m_ActiveCommandList);
			m_ActiveCommandList.reset();
		}

        std::shared_ptr<InternalCommandList> CommandList::createInternalCommandList() const {
            auto commandList = std::make_shared<InternalCommandList>();

            D3D12_COMMAND_LIST_TYPE d3dCommandListType;
            switch (m_Desc.queueType)
            {
            case CommandQueue::Graphics:
                d3dCommandListType = D3D12_COMMAND_LIST_TYPE_DIRECT;
                break;
            case CommandQueue::Compute:
                d3dCommandListType = D3D12_COMMAND_LIST_TYPE_COMPUTE;
                break;
            case CommandQueue::Copy:
                d3dCommandListType = D3D12_COMMAND_LIST_TYPE_COPY;
                break;

            case CommandQueue::Count:
            default:
                return nullptr;
            }

            m_device->CreateCommandAllocator(d3dCommandListType, IID_PPV_ARGS(&commandList->allocator));
            m_device->CreateCommandList(0, d3dCommandListType, commandList->allocator.Get(), nullptr, IID_PPV_ARGS(&commandList->commandList));

            return commandList;
        }

		void CommandList::open() {
            uint64_t completedInstance = m_Queue->updateLastCompletedInstance();

            std::shared_ptr<InternalCommandList> chunk;

            if (!m_CommandListPool.empty())
            {
                chunk = m_CommandListPool.front();

                if (chunk->lastSubmittedInstance <= completedInstance)
                {
                    chunk->allocator->Reset();
                    chunk->commandList->Reset(chunk->allocator.Get(), nullptr);
                    m_CommandListPool.pop_front();
                }
                else
                {
                    chunk = nullptr;
                }
            }

            if (chunk == nullptr)
            {
                chunk = createInternalCommandList();
            }

            m_ActiveCommandList = chunk;
		}

        void CommandList::close() {
            m_ActiveCommandList->commandList->Close();
        }

        void* CommandList::getNativePtr() const noexcept {
            return m_ActiveCommandList->commandList.Get();
        }

	}
}