#pragma once
#include "core.h"
#include "rhi.h"
#include <string>
namespace fccs {
	namespace framegraph {
		class Pass {
		public:
			Pass(rhi::IDevice* pDevice, std::string name) : m_Device(pDevice), m_passName(name) {}
			virtual ~Pass(){}
		private:
			rhi::IDevice* m_Device;
			std::string m_passName;
		};

		class FrameGraph {

		};
	}
}