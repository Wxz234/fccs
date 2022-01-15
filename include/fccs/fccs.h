#pragma once
#define FCCS_API
#include <memory>
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <string>
#include <d3d12.h>
#define FCCS_BITMASK_OPS(_BITMASK)																					\
[[nodiscard]] constexpr _BITMASK operator&(_BITMASK _Left, _BITMASK _Right) noexcept {								\
    using _IntTy = ::std::underlying_type_t<_BITMASK>;																\
    return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) & static_cast<_IntTy>(_Right));							\
}																													\
																													\
[[nodiscard]] constexpr _BITMASK operator|(_BITMASK _Left, _BITMASK _Right) noexcept { /* return _Left | _Right */	\
    using _IntTy = ::std::underlying_type_t<_BITMASK>;																\
    return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) | static_cast<_IntTy>(_Right));							\
}																													\
																													\
[[nodiscard]] constexpr _BITMASK operator^(_BITMASK _Left, _BITMASK _Right) noexcept { /* return _Left ^ _Right */	\
    using _IntTy = ::std::underlying_type_t<_BITMASK>;																\
    return static_cast<_BITMASK>(static_cast<_IntTy>(_Left) ^ static_cast<_IntTy>(_Right));							\
}																													\
																													\
constexpr _BITMASK& operator&=(_BITMASK& _Left, _BITMASK _Right) noexcept { /* return _Left &= _Right */			\
    return _Left = _Left & _Right;																					\
}																													\
																													\
constexpr _BITMASK& operator|=(_BITMASK& _Left, _BITMASK _Right) noexcept { /* return _Left |= _Right */			\
    return _Left = _Left | _Right;																					\
}																													\
																													\
constexpr _BITMASK& operator^=(_BITMASK& _Left, _BITMASK _Right) noexcept { /* return _Left ^= _Right */			\
    return _Left = _Left ^ _Right;																					\
}																													\
																													\
[[nodiscard]] constexpr _BITMASK operator~(_BITMASK _Left) noexcept { /* return ~_Left */							\
    using _IntTy = ::std::underlying_type_t<_BITMASK>;																\
    return static_cast<_BITMASK>(~static_cast<_IntTy>(_Left));														\
}                                                                                                               



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
			Unknown					= 0,
			Common					= 0x00000001,
			ConstantBuffer			= 0x00000002,
			VertexBuffer			= 0x00000004,
			IndexBuffer				= 0x00000008,
			IndirectArgument		= 0x00000010,
			ShaderResource			= 0x00000020,
			UnorderedAccess			= 0x00000040,
			RenderTarget			= 0x00000080,
			DepthWrite				= 0x00000100,
			DepthRead				= 0x00000200,
			StreamOut				= 0x00000400,
			CopyDest				= 0x00000800,
			CopySource				= 0x00001000,
			ResolveDest				= 0x00002000,
			ResolveSource			= 0x00004000,
			Present					= 0x00008000,
			AccelStructRead			= 0x00010000,
			AccelStructWrite		= 0x00020000,
			AccelStructBuildInput	= 0x00040000,
			AccelStructBuildBlas	= 0x00080000,
			ShadingRateSurface		= 0x00100000,
		};
		FCCS_BITMASK_OPS(ResourceStates)

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
		FCCS_API DeviceHandle CreateDeivce(const DeviceDesc& desc = DeviceDesc());
	}

	namespace window {
		class ISwapChain :public IResource {
		public:
		};
		typedef SharedPtr<ISwapChain> SwapChainHandle;
		struct WindowDesc
		{
			uint32_t width = 800;
			uint32_t height = 600;
			std::wstring title = L"fccs";
		};

		class IWindowCallback {
		public:
			virtual void Initialize() = 0;
			virtual void Update() = 0;
			virtual void Release() = 0;
		};

		class IWindow : public IResource {
		public:
			virtual void OpenWindow() = 0;
			virtual int32_t Run(IWindowCallback* callback) = 0;
		};

		typedef SharedPtr<IWindow> WindowHandle;

		FCCS_API WindowHandle CreateDeivce(const WindowDesc& desc = WindowDesc());
	}
}
