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
			addCommandAllocatorRefCount();
		}
		CommandList::~CommandList() {
			releaseCommandAllocatorRef();
		}

		void CommandList::ChangeTextureState(ITexture* pTexure, ResourceStates state) {
			ID3D12Resource* res = (ID3D12Resource*)pTexure->GetNativePtr();
			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.pResource = res;
			barrier.Transition.StateAfter = convertResourceStates(state);
			barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
			if (m_texture_state.find(pTexure) == m_texture_state.end())
				barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
			else
				barrier.Transition.StateBefore = convertResourceStates(m_texture_state[pTexure]);

			m_texture_state[pTexure] = state;

			m_mainLists->ResourceBarrier(1, &barrier);
		}

		void CommandList::Open() {
			m_Alloc = getCommandAllocatorInPool(m_device.Get(), m_Desc);
			m_mainLists->Reset(m_Alloc.Get(), nullptr);
		}

		void CommandList::Close() {
			for (auto state: m_texture_state) {
				D3D12_RESOURCE_BARRIER barrier = {};
				barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
				barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
				barrier.Transition.pResource = (ID3D12Resource*)state.first->GetNativePtr();
				barrier.Transition.StateBefore = convertResourceStates(state.second);
				barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
				barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
				m_mainLists->ResourceBarrier(1, &barrier);
			}

			m_texture_state.clear();
			m_mainLists->Close();
		}

	}
}