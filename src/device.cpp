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

			if (desc.pGraphicsCommandQueue) {
				m_GraphicsCommandQueue = desc.pGraphicsCommandQueue;
			}
			else {
				D3D12_COMMAND_QUEUE_DESC queueDesc = {};
				queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
				queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
				m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_GraphicsCommandQueue));
			}

			if (desc.pComputeCommandQueue) {
				m_ComputeCommandQueue = desc.pComputeCommandQueue;
			}
			else {
				D3D12_COMMAND_QUEUE_DESC queueDesc = {};
				queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
				queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COMPUTE;
				m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_ComputeCommandQueue));
			}

			if (desc.pCopyCommandQueue) {
				m_CopyCommandQueue = desc.pCopyCommandQueue;
			}
			else {
				D3D12_COMMAND_QUEUE_DESC queueDesc = {};
				queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
				queueDesc.Type = D3D12_COMMAND_LIST_TYPE_COPY;
				m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CopyCommandQueue));
			}

			m_fenceEvent.Attach(CreateEvent(nullptr, false, false, nullptr));
		}
		Device::~Device() {
			waitForIdle();
		}

		void Device::waitForIdle() {

		}

		CommandListHandle Device::createCommandList(const CommandListParameters& params) {
			return CommandListHandle();
		}
		void* Device::getNativePtr() const noexcept {
			return m_device.Get();
		}

		FCCS_API DeviceHandle createDeivce(const DeviceDesc& desc) {
			return DeviceHandle(new Device(desc));
		}
	}
}