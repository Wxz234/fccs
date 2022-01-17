#pragma once
#define FCCS_API

#include <memory>
#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <string>
#include <d3d12.h>
#include <windef.h>
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
	enum class Format : uint8_t
	{
		UNKNOWN,
		R8_UINT,
		R8_SINT,
		R8_UNORM,
		R8_SNORM,
		RG8_UINT,
		RG8_SINT,
		RG8_UNORM,
		RG8_SNORM,
		R16_UINT,
		R16_SINT,
		R16_UNORM,
		R16_SNORM,
		R16_FLOAT,
		BGRA4_UNORM,
		B5G6R5_UNORM,
		B5G5R5A1_UNORM,
		RGBA8_UINT,
		RGBA8_SINT,
		RGBA8_UNORM,
		RGBA8_SNORM,
		BGRA8_UNORM,
		SRGBA8_UNORM,
		SBGRA8_UNORM,
		R10G10B10A2_UNORM,
		R11G11B10_FLOAT,
		RG16_UINT,
		RG16_SINT,
		RG16_UNORM,
		RG16_SNORM,
		RG16_FLOAT,
		R32_UINT,
		R32_SINT,
		R32_FLOAT,
		RGBA16_UINT,
		RGBA16_SINT,
		RGBA16_FLOAT,
		RGBA16_UNORM,
		RGBA16_SNORM,
		RG32_UINT,
		RG32_SINT,
		RG32_FLOAT,
		RGB32_UINT,
		RGB32_SINT,
		RGB32_FLOAT,
		RGBA32_UINT,
		RGBA32_SINT,
		RGBA32_FLOAT,
		D16,
		D24S8,
		X24G8_UINT,
		D32,
		D32S8,
		X32G8_UINT,
		BC1_UNORM,
		BC1_UNORM_SRGB,
		BC2_UNORM,
		BC2_UNORM_SRGB,
		BC3_UNORM,
		BC3_UNORM_SRGB,
		BC4_UNORM,
		BC4_SNORM,
		BC5_UNORM,
		BC5_SNORM,
		BC6H_UFLOAT,
		BC6H_SFLOAT,
		BC7_UNORM,
		BC7_UNORM_SRGB,
		COUNT,
	};


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
			virtual ID3D12CommandQueue* GetNativeQueuePtr() const noexcept = 0;
			virtual CommandListHandle CreateCommandList(CommandQueueType type) = 0;
			virtual CommandQueueHandle CreateCommandQueue(CommandQueueType type) = 0;
		};

		typedef SharedPtr<IDevice> DeviceHandle;
		FCCS_API DeviceHandle CreateDeivce(const DeviceDesc& desc = DeviceDesc());
	}

	namespace window {
		class ISwapChain :public IResource {
		public:
			virtual void Present(uint32_t sync) = 0;
		};
		typedef SharedPtr<ISwapChain> SwapChainHandle;

		struct SwapChainDesc
		{
			uint32_t width;
			uint32_t height;
			HWND hwnd;
		};
		FCCS_API SwapChainHandle CreateSwapChain(const SwapChainDesc& desc, rhi::ICommandQueue* pQueue);

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
			virtual HWND GetHWND() const = 0;
			virtual void OpenWindow() = 0;
			virtual int32_t Run(IWindowCallback* callback) = 0;
		};

		typedef SharedPtr<IWindow> WindowHandle;

		FCCS_API WindowHandle CreateFCCSWindow(const WindowDesc& desc = WindowDesc());
	}
}
