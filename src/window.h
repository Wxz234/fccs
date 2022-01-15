#pragma once
#include <fccs/fccs.h>
#include <Windows.h>

namespace fccs {
	namespace window {
		class Window : public IWindow {
		public:
			Window(const WindowDesc& desc);
			void OpenWindow();
			int32_t Run(IWindowCallback* callback);
			WindowDesc m_Desc;
			HWND m_hwnd;

			window::IWindowCallback* m_callback = nullptr;
            MSG msg = {};
		};
	}
}