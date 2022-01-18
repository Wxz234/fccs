#pragma once
#include <fccs/window.h>
namespace fccs {
	namespace window {
		class Window : public IWindow {
		public:
			Window(const WindowDesc& desc);
			void OpenWindow();

			HWND GetHWND() const noexcept {
				return m_hwnd;
			}
			WindowDesc m_Desc;
			HWND m_hwnd;

		};
	}
}