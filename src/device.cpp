#include "device.h"

#include "dx12_backend.h"
namespace fccs {
	namespace rhi {

		Device::Device(const DeviceDesc& desc) {
			if (desc.pDevice) {
				m_device = desc.pDevice;
			}
			else {
				createD3D12Device(&m_device);
			}
		}
		Device::~Device() {

		}

		void Device::waitForIdle() {

		}



		void* Device::getNativePtr() const noexcept {
			return m_device.Get();
		}


		FCCS_API DeviceHandle createDeivce(const DeviceDesc& desc) {
			return DeviceHandle(new Device(desc));
		}
	}
}