#pragma once
#include <fccs/fccs.h>

#include <Windows.h>
namespace fccs {
	namespace window {
		class Window : public IWindow {
		public:
			Window(const WindowDesc& desc) : m_Desc(desc) {}
			WindowDesc m_Desc;

			bool s_in_sizemove = false;
			bool s_in_suspend = false;
			bool s_minimized = false;
			bool s_fullscreen = false;
		};
	}
}