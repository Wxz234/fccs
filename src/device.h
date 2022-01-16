#pragma once
#include <fccs/fccs.h>

#include <wrl/client.h>

namespace fccs{
	namespace rhi {
		class Device : public IDevice {
		public:
			Device(const DeviceDesc& desc);
			~Device();
			ID3D12CommandQueue* GetNativeQueuePtr() const noexcept;
			CommandListHandle CreateCommandList(CommandQueueType type);
			CommandQueueHandle CreateCommandQueue(CommandQueueType type);
			void waitForIdle();
			void* GetNativePtr() const noexcept;
			Microsoft::WRL::ComPtr<ID3D12Device> m_device;
			CommandQueueHandle m_cmd_queue;
		};

	}
}