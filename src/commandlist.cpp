#include "commandlist.h"

namespace fccs {
	namespace rhi {
		CommandList::CommandList(ID3D12Device* pDevice, QueueRef* pQueue, CommandListParameters parameters) {
			m_device = pDevice;
			m_queue = pQueue;
			m_para = parameters;
		}

		void* CommandList::getNativePtr() const noexcept {
			return nullptr;
		}
		std::shared_ptr<InternalCommandList> CommandList::createInternalCommandList() const {
			auto commandList = std::make_shared<InternalCommandList>();

			D3D12_COMMAND_LIST_TYPE d3dCommandListType;
			switch (m_para.queueType)
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

		}

		void CommandList::close() {
			m_ActiveCommandList->commandList->Close();
		}

	}
}