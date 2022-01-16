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
			m_cmd_queue = CreateCommandQueue(CommandQueueType::Graphics);
		}
		Device::~Device() {
			m_cmd_queue.reset();
		}

		void Device::waitForIdle() {

		}

		ID3D12CommandQueue* Device::GetNativeQueuePtr() const noexcept {
			ID3D12CommandQueue* temp = (ID3D12CommandQueue*)(m_cmd_queue->GetNativePtr());
			return temp;
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