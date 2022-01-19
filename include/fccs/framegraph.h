#pragma once
#include "core.h"
#include "rhi.h"
#include <string>
#include <vector>

namespace fccs {
	namespace framegraph {

		class ResourceNode {
		public:
			std::string name;
		};

		class PassNode {
		public:
			PassNode(rhi::IDevice* pDevice, std::string name) : m_Device(pDevice), m_passName(name) {}
			virtual ~PassNode(){}
		private:
			std::vector<uint32_t> input_resource;
			std::vector<uint32_t> output_resource;
			rhi::IDevice* m_Device;
			std::string m_passName;
		};

		class FrameGraph {
		public:
			FrameGraph(rhi::IDevice* pDevice) {}
			uint32_t AddResourceNode(std::string name, rhi::Format format, uint32_t width, uint32_t height, Color color) {
				return 0;
			}

		private:
			struct InteranlResourceNode {

			};
			std::vector<InteranlResourceNode> m_res;
			uint32_t m_resource_index = 0;
		};
	}
}