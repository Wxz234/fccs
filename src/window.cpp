#include "window.h"
#include <mutex>

namespace fccs {

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        static bool s_in_sizemove = false;
        static bool s_in_suspend = false;
        static bool s_minimized = false;
        static bool s_fullscreen = false;
        // TODO: Set s_fullscreen to true if defaulting to fullscreen.

        auto game = reinterpret_cast<window::IWindowCallback*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

        switch (message)
        {
        case WM_PAINT:
            if (s_in_sizemove && game)
            {
                game->Update();
            }
            else
            {
                PAINTSTRUCT ps;
                (void)BeginPaint(hWnd, &ps);
                EndPaint(hWnd, &ps);
            }
            break;

        case WM_SIZE:
            if (wParam == SIZE_MINIMIZED)
            {
                if (!s_minimized)
                {
                    s_minimized = true;
                    if (!s_in_suspend && game) {
                        //game->OnSuspending();
                    }
                       
                    s_in_suspend = true;
                }
            }
            else if (s_minimized)
            {
                s_minimized = false;
                if (s_in_suspend && game) {
                    //game->OnResuming();
                }
                s_in_suspend = false;
            }
            else if (!s_in_sizemove && game)
            {
                //game->OnWindowSizeChanged(LOWORD(lParam), HIWORD(lParam));
            }
            break;

        case WM_ENTERSIZEMOVE:
            s_in_sizemove = true;
            break;

        case WM_EXITSIZEMOVE:
            s_in_sizemove = false;
            if (game)
            {
                RECT rc;
                GetClientRect(hWnd, &rc);

                //game->OnWindowSizeChanged(rc.right - rc.left, rc.bottom - rc.top);
            }
            break;

        case WM_GETMINMAXINFO:
            if (lParam)
            {
                auto info = reinterpret_cast<MINMAXINFO*>(lParam);
                info->ptMinTrackSize.x = 320;
                info->ptMinTrackSize.y = 200;
            }
            break;

        case WM_ACTIVATEAPP:
            if (game)
            {
                if (wParam)
                {
                    //game->OnActivated();
                }
                else
                {
                    //game->OnDeactivated();
                }
            }
            break;

        case WM_POWERBROADCAST:
            switch (wParam)
            {
            case PBT_APMQUERYSUSPEND:
                if (!s_in_suspend && game) {
                    //game->OnSuspending();
                }
                s_in_suspend = true;
                return TRUE;

            case PBT_APMRESUMESUSPEND:
                if (!s_minimized)
                {
                    if (s_in_suspend && game) {
                        //game->OnResuming();
                    }
                        
                    s_in_suspend = false;
                }
                return TRUE;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_SYSKEYDOWN:
            //if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
            //{
            //    // Implements the classic ALT+ENTER fullscreen toggle
            //    if (s_fullscreen)
            //    {
            //        SetWindowLongPtr(hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
            //        SetWindowLongPtr(hWnd, GWL_EXSTYLE, 0);

            //        int width = 800;
            //        int height = 600;
            //        if (game)
            //        {
            //            //game->GetDefaultSize(width, height);
            //        }
            //            

            //        ShowWindow(hWnd, SW_SHOWNORMAL);

            //        SetWindowPos(hWnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
            //    }
            //    else
            //    {
            //        SetWindowLongPtr(hWnd, GWL_STYLE, WS_POPUP);
            //        SetWindowLongPtr(hWnd, GWL_EXSTYLE, WS_EX_TOPMOST);

            //        SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

            //        ShowWindow(hWnd, SW_SHOWMAXIMIZED);
            //    }

            //    s_fullscreen = !s_fullscreen;
            //}
            break;

        case WM_MENUCHAR:
            // A menu is active and the user presses a key that does not correspond
            // to any mnemonic or accelerator key. Ignore so we don't produce an error beep.
            return MAKELRESULT(0, MNC_CLOSE);
        }

        return DefWindowProc(hWnd, message, wParam, lParam);
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
            }
        );
	}

    namespace window {
        Window::Window(const WindowDesc& desc) {
            m_Desc = desc;
            RegisterClassOnce();
            RECT rc = { 0, 0, static_cast<LONG>(m_Desc.width), static_cast<LONG>(m_Desc.height) };
            AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, FALSE);
            m_hwnd = CreateWindowExW(0, m_Desc.title.c_str(), L"fccs_class", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
                CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, GetModuleHandle(nullptr),
                nullptr);
        }
        void Window::OpenWindow() {
            ShowWindow(m_hwnd, SW_SHOWDEFAULT);
        }

        int32_t Window::Run(IWindowCallback* callback) {
            m_callback = callback;
            m_callback->Initialize();
           
            SetWindowLongPtr(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(callback));
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
            m_callback->Release();
            return static_cast<int>(msg.wParam);
        }
    }
}