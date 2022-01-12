#pragma once
#include <fccs/fccs.h>

#include <wrl/client.h>

namespace fccs{
	namespace rhi {
		class Device : public IDevice {
		public:
			Device(const DeviceDesc& desc);
			~Device();

			CommandListHandle createCommandList(CommandQueueType type);
			CommandQueueHandle createCommandQueue(CommandQueueType type);
			void waitForIdle();
			void* getNativePtr() const noexcept;
			Microsoft::WRL::ComPtr<ID3D12Device> m_device;

		};

	}
}