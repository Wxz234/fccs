#pragma once
#include "core.h"
#include <cstdint>
#include <string>
#include <Windows.h>
namespace fccs {
	namespace window {

		struct WindowDesc
		{
			uint32_t width = 800;
			uint32_t height = 600;
			std::wstring title = L"fccs";
		};

		class IWindow : public IResource {
		public:
			virtual HWND GetHWND() const noexcept = 0;
			virtual void OpenWindow() = 0;
		private:
		};
		FCCS_API IWindow* CreateFCCSWindow(const WindowDesc& desc);
	}
}