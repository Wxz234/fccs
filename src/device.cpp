#include "device.h"
#include "commandlist.h"
#include "commandqueue.h"
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

		CommandListHandle Device::CreateCommandList(CommandQueueType type) {
			return CommandListHandle(new CommandList(m_device.Get(), type));
		}
		CommandQueueHandle Device::CreateCommandQueue(CommandQueueType type) {
			return CommandQueueHandle(new CommandQueue(m_device.Get(), type));
		}



		void* Device::GetNativePtr() const noexcept {
			return m_device.Get();
		}


		FCCS_API DeviceHandle CreateDeivce(const DeviceDesc& desc) {
			return DeviceHandle(new Device(desc));
		}
	}
}