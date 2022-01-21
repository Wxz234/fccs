#pragma once
#include "core.h"
#include <cstdint>
#include <d3d12.h>
#include <windef.h>
namespace fccs {
	namespace rhi {

		class IRHIObject : public IResource {
		public:
			virtual void* GetNativePtr() const noexcept = 0;
		};

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

		enum class ResourceStates : uint32_t {
			Unknown = 0,
			Common = 0x00000001,
			ConstantBuffer = 0x00000002,
			VertexBuffer = 0x00000004,
			IndexBuffer = 0x00000008,
			IndirectArgument = 0x00000010,
			ShaderResource = 0x00000020,
			UnorderedAccess = 0x00000040,
			RenderTarget = 0x00000080,
			DepthWrite = 0x00000100,
			DepthRead = 0x00000200,
			StreamOut = 0x00000400,
			CopyDest = 0x00000800,
			CopySource = 0x00001000,
			ResolveDest = 0x00002000,
			ResolveSource = 0x00004000,
			Present = 0x00008000,
			AccelStructRead = 0x00010000,
			AccelStructWrite = 0x00020000,
			AccelStructBuildInput = 0x00040000,
			AccelStructBuildBlas = 0x00080000,
			ShadingRateSurface = 0x00100000,
		};
		FCCS_BITMASK_OPS(ResourceStates)


        enum class TextureDimension : uint8_t
        {
            Texture1D,
            Texture1DArray,
            Texture2D,
            Texture2DArray,
            TextureCube,
            TextureCubeArray,
            Texture2DMS,
            Texture2DMSArray,
            Texture3D
        };

        struct TextureDesc
        {
            uint32_t width = 1;
            uint32_t height = 1;
            uint32_t depth = 1;
            uint32_t arraySize = 1;
            uint32_t mipLevels = 1;
            uint32_t sampleCount = 1;
            uint32_t sampleQuality = 0;
            Format format = Format::UNKNOWN;
            TextureDimension dimension = TextureDimension::Texture2D;
            bool isRenderTarget = false;
            bool isUAV = false;
            bool isTypeless = false;
            Color clearValue;
            bool useClearValue = false;
            ResourceStates initialState = ResourceStates::Unknown;

            constexpr TextureDesc& SetWidth(uint32_t value) { width = value; return *this; }
            constexpr TextureDesc& SetHeight(uint32_t value) { height = value; return *this; }
            constexpr TextureDesc& SetDepth(uint32_t value) { depth = value; return *this; }
            constexpr TextureDesc& SetArraySize(uint32_t value) { arraySize = value; return *this; }
            constexpr TextureDesc& SetMipLevels(uint32_t value) { mipLevels = value; return *this; }
            constexpr TextureDesc& SetSampleCount(uint32_t value) { sampleCount = value; return *this; }
            constexpr TextureDesc& SetSampleQuality(uint32_t value) { sampleQuality = value; return *this; }
            constexpr TextureDesc& SetFormat(Format value) { format = value; return *this; }
            constexpr TextureDesc& SetDimension(TextureDimension value) { dimension = value; return *this; }
            constexpr TextureDesc& SetIsRenderTarget(bool value) { isRenderTarget = value; return *this; }
            constexpr TextureDesc& SetIsUAV(bool value) { isUAV = value; return *this; }
            constexpr TextureDesc& SetIsTypeless(bool value) { isTypeless = value; return *this; }
            constexpr TextureDesc& SetClearValue(const Color& value) { clearValue = value; useClearValue = true; return *this; }
            constexpr TextureDesc& SetUseClearValue(bool value) { useClearValue = value; return *this; }
            constexpr TextureDesc& SetInitialState(ResourceStates value) { initialState = value; return *this; }
        };
        class ITexture : public IRHIObject {};
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

		class ICommandQueue : public IRHIObject {
		public:
			virtual void ExecuteCommandLists(uint32_t NumCommandLists, ICommandList* const* ppCommandLists) = 0;
		};

		struct DeviceDesc
		{
			ID3D12Device* pDevice = nullptr;
		};

		class IDevice : public IRHIObject {
		public:
            virtual ITexture* CreateTexture(const TextureDesc& desc) = 0;
			virtual ICommandList* CreateCommandList(CommandQueueType type) = 0;
			virtual ICommandQueue* CreateCommandQueue(CommandQueueType type) = 0;
		};

		FCCS_API IDevice* CreateDeivce(const DeviceDesc& desc = DeviceDesc());

		class ISwapChain : public IResource {
		public:
			virtual uint32_t GetCurrentBackBufferIndex() const noexcept = 0;
			virtual ID3D12Resource* GetNativeResourcePtr(uint32_t n) const noexcept = 0;
			virtual D3D12_CPU_DESCRIPTOR_HANDLE GetRenderTargetView(uint32_t n) const noexcept = 0;
			virtual void Present(uint32_t sync) = 0;
		};

        FCCS_API ISwapChain* CreateSwapChain(ICommandQueue* pQueue, uint32_t width, uint32_t height, HWND hwnd);
	}
}