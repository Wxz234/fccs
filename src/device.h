#pragma once
#include <fccs/fccs.h>

#include <wrl/client.h>

namespace fccs{
	namespace rhi {
		class Device : public IDevice {
		public:
			Device(const DeviceDesc& desc);
			~Device();

			void waitForIdle();
			void* getNativePtr() const noexcept;
			Microsoft::WRL::ComPtr<ID3D12Device> m_device;

		};

	}
}