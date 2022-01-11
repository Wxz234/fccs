#pragma once
#define FCCS_API
#include <memory>
#include <cstdint>
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

		enum class CommandQueue : uint8_t
		{
			Graphics = 0,
			Compute,
			Copy,

			Count,
		};

		struct CommandListParameters {
			CommandQueue queueType = CommandQueue::Graphics;
		};


		class ICommandList : public IRHIObject {
		public:
		};

		typedef SharedPtr<ICommandList> CommandListHandle;

		struct DeviceDesc
		{
			ID3D12Device* pDevice = nullptr;
			ID3D12CommandQueue* pGraphicsCommandQueue = nullptr;
			ID3D12CommandQueue* pComputeCommandQueue = nullptr;
			ID3D12CommandQueue* pCopyCommandQueue = nullptr;
		};

		class IDevice : public IRHIObject {
		public:
			virtual CommandListHandle createCommandList(const CommandListParameters& params = CommandListParameters()) = 0;
		};

		typedef SharedPtr<IDevice> DeviceHandle;
		FCCS_API DeviceHandle createDeivce(const DeviceDesc& desc);
	}
}
