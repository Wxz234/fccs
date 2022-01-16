#include "commandlist.h"
#include "dx12_backend.h"
namespace fccs {
	namespace rhi {
		CommandList::CommandList(ID3D12Device* pDevice, CommandQueueType type) {
			m_device = pDevice;
			m_Alloc = nullptr;
			m_Desc = type;
			ID3D12Device8* device8 = nullptr;
			m_device->QueryInterface(&device8);
			if (type == CommandQueueType::Graphics) {
				device8->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&m_mainLists));
			}
			else if (type == CommandQueueType::Compute) {
				device8->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_COMPUTE, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&m_mainLists));
			}
			else {
				device8->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_COPY, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&m_mainLists));
			}
			device8->Release();
		}

		void CommandList::Open() {
			m_Alloc = getCommandAllocatorInPool(m_device.Get(), m_Desc);
			m_mainLists->Reset(m_Alloc.Get(), nullptr);
		}

		void CommandList::Close() {
			m_mainLists->Close();
		}

	}
}