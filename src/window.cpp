#include "window.h"
#include <mutex>

namespace fccs {

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        window::Window* window_ptr = reinterpret_cast<window::Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        switch (message)
        {
        case WM_CREATE:
            {
                LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
            }
            break;
        case WM_PAINT:
            {
                if (window_ptr && window_ptr->m_callback) {
                    window_ptr->m_callback->Update();
                }
                else {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hWnd, &ps);
                    // TODO: 在此处添加使用 hdc 的任何绘图代码...
                    EndPaint(hWnd, &ps);
                }
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }


	void RegisterClassOnce() {
        static std::once_flag oc;
        std::call_once(oc, 
            []() {
                WNDCLASSEXW wcex = {};
                wcex.cbSize = sizeof(WNDCLASSEXW);
                wcex.style = CS_HREDRAW | CS_VREDRAW;
                wcex.lpfnWndProc = WndProc;
                wcex.hInstance = GetModuleHandle(nullptr);
                wcex.hIcon = LoadIconW(wcex.hInstance, L"IDI_ICON");
                wcex.hCursor = LoadCursorW(nullptr, IDC_ARROW);
                wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
                wcex.lpszClassName = L"fccs_class";
                wcex.hIconSm = LoadIconW(wcex.hInstance, L"IDI_ICON");

                RegisterClassExW(&wcex);
            }
        );
	}

    namespace window {
        Window::Window(const WindowDesc& desc) {
            m_Desc = desc;
            RegisterClassOnce();
            RECT rc = { 0, 0, static_cast<LONG>(m_Desc.width), static_cast<LONG>(m_Desc.height) };
            AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, FALSE);
            m_hwnd = CreateWindowExW(0, L"fccs_class", m_Desc.title.c_str(), WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
                CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, GetModuleHandle(nullptr),
                this);
        }
        void Window::OpenWindow() {
            ShowWindow(m_hwnd, SW_SHOWDEFAULT);
        }

        int32_t Window::Run(IWindowCallback* callback) {

            callback->Initialize();
            m_callback = callback;
            while (WM_QUIT != msg.message)
            {
                if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
                else
                {
                    m_callback->Update();
                }
            }
            m_callback = nullptr;
            callback->Release();
            return static_cast<int>(msg.wParam);
        }

        FCCS_API WindowHandle CreateFCCSWindow(const WindowDesc& desc) {
            return WindowHandle(new Window(desc));
        }
    }
}