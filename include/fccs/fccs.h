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
			virtual void* GetNativePtr() const noexcept = 0;
		};

		enum class ResourceStates : uint32_t {
			Unknown			= 0
		};

		enum class CommandQueueType : uint8_t
		{
			Graphics = 0,
			Compute,
			Copy,
		};

		class ICommandList : public IRHIObject {
		public:
			virtual void Open() = 0;
			virtual void Close() = 0;
		};
		typedef SharedPtr<ICommandList> CommandListHandle;

		class ICommandQueue : public IRHIObject {
		public:
			virtual void ExecuteCommandLists(uint32_t NumCommandLists, ICommandList* const* ppCommandLists) = 0;
		};
		typedef SharedPtr<ICommandQueue> CommandQueueHandle;

		struct DeviceDesc
		{
			ID3D12Device* pDevice = nullptr;
		};

		class IDevice : public IRHIObject {
		public:
			virtual CommandListHandle CreateCommandList(CommandQueueType type) = 0;
			virtual CommandQueueHandle CreateCommandQueue(CommandQueueType type) = 0;
		};

		typedef SharedPtr<IDevice> DeviceHandle;
		FCCS_API DeviceHandle CreateDeivce(const DeviceDesc& desc);
	}
}
