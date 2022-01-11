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

	}
}