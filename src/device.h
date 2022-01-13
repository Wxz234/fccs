#pragma once
#include <fccs/fccs.h>

#include <wrl/client.h>

namespace fccs{
	namespace rhi {
		class Device : public IDevice {
		public:
			Device(const DeviceDesc& desc);
			~Device();

			CommandListHandle CreateCommandList(CommandQueueType type);
			CommandQueueHandle CreateCommandQueue(CommandQueueType type);
			void waitForIdle();
			void* GetNativePtr() const noexcept;
			Microsoft::WRL::ComPtr<ID3D12Device> m_device;

		};

	}
}