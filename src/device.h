#pragma once
#include <fccs/fccs.h>

#include <wrl.h>

namespace fccs{
	namespace rhi {
		class Device : public IDevice {
		public:
			Device(const DeviceDesc& desc);
			~Device();

			void waitForIdle();
			CommandListHandle Device::createCommandList(const CommandListParameters& params);
			void* getNativePtr() const noexcept;

			Microsoft::WRL::ComPtr<ID3D12Device> m_device;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_GraphicsCommandQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_ComputeCommandQueue;
			Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_CopyCommandQueue;

			Microsoft::WRL::Wrappers::Event m_fenceEvent;
		};

	}
}