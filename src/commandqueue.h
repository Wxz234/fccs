#pragma once
#include <fccs/fccs.h>

#include <wrl.h>
#include <cstdint>

namespace fccs {
	namespace rhi {
		class CommandQueue : public ICommandQueue {
		public:
			CommandQueue(ID3D12Device* pDevice, CommandQueueType type);
			~CommandQueue();
			void ExecuteCommandLists(uint32_t NumCommandLists, ICommandList* const* ppCommandLists);
			uint64_t getCompletedValue();
			void* GetNativePtr() const noexcept {
				return m_queue.Get();
			}
			ID3D12Device* m_device;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_queue;
			Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
			Microsoft::WRL::Wrappers::Event m_event;
			uint64_t submitValue;
		};

	}
}