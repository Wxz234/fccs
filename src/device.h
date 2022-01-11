#pragma once
#include <fccs/fccs.h>

#include <wrl.h>
#include <array>
#include "dx12_backend.h"

namespace fccs{
	namespace rhi {
		class Device : public IDevice {
		public:
			Device(const DeviceDesc& desc);
			~Device();

			void waitForIdle();
	
			void executeCommandLists(ICommandList* const* pCommandLists, size_t numCommandLists);
			void* getNativePtr() const noexcept;

			Microsoft::WRL::ComPtr<ID3D12Device> m_device;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_GraphicsCommandQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_ComputeCommandQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CopyCommandQueue;

			Microsoft::WRL::Wrappers::Event m_fenceEvent;

			std::array<std::unique_ptr<QueueRef>, (int)CommandQueue::Count> m_Queues;
		};

	}
}