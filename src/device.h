#pragma once
#include <fccs/rhi.h>
#include <wrl/client.h>
namespace fccs{
	namespace rhi {
		class Device : public IDevice {
		public:
			Device(const DeviceDesc& desc);
			~Device();
			ITexture* CreateTexture(const TextureDesc& desc);
			ICommandList* CreateCommandList(CommandQueueType type);
			ICommandQueue* CreateCommandQueue(CommandQueueType type);
			void waitForIdle();
			void* GetNativePtr() const noexcept;
			Microsoft::WRL::ComPtr<ID3D12Device> m_device;

		};

	}
}