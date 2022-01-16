#pragma once
#include <fccs/fccs.h>
#include <dxgi1_6.h>
namespace fccs {
	namespace window {
		class SwapChain : public ISwapChain {
		public:
			SwapChain();
			void Present(uint32_t sync) {}
		};
	}
}