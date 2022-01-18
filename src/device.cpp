#include "device.h"
#include "dx12_backend.h"
#include "commandlist.h"
#include "commandqueue.h"
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
		Device::~Device() {}

		void Device::waitForIdle() {}



		ICommandList* Device::CreateCommandList(CommandQueueType type) {
			return new CommandList(m_device.Get(), type);
		}
		ICommandQueue* Device::CreateCommandQueue(CommandQueueType type) {
			return new CommandQueue(m_device.Get(), type);
		}



		void* Device::GetNativePtr() const noexcept {
			return m_device.Get();
		}


		FCCS_API IDevice* CreateDeivce(const DeviceDesc& desc) {
			return new Device(desc);
		}
	}
}