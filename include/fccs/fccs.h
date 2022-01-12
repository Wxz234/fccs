#pragma once
#define FCCS_API
#include <memory>
#include <cstdint>
#include <cstddef>
#include <d3d12.h>
namespace fccs {

	template<typename T>
	using SharedPtr = std::shared_ptr<T>;

	class IResource {
	public:
		virtual ~IResource();
	};

	namespace rhi {

		class IRHIObject : public IResource {
		public:
			virtual void* getNativePtr() const noexcept = 0;
		};

		enum class CommandQueueType : uint8_t
		{
			Graphics = 0,
			Compute,
			Copy,

			Count,
		};

		class ICommandQueue : public IRHIObject {
		public:
		};
		typedef SharedPtr<ICommandQueue> CommandQueueHandle;

		class ICommandList : public IRHIObject {
		public:
			virtual void open() = 0;
			virtual void close() = 0;
		};

		typedef SharedPtr<ICommandList> CommandListHandle;

		struct DeviceDesc
		{
			ID3D12Device* pDevice = nullptr;
		};

		class IDevice : public IRHIObject {
		public:
		};

		typedef SharedPtr<IDevice> DeviceHandle;
		FCCS_API DeviceHandle createDeivce(const DeviceDesc& desc);
	}
}
