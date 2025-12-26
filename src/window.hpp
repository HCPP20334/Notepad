#pragma once
#include <Windows.h>
#include <string_view>
#include "imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Window
{
    class WindowClass
    {
    public:
        static constexpr std::wstring_view window_title = L"Notepad";

    public:
        WindowClass()
        {
            wc       = { sizeof(wc), CS_OWNDC, Window::WndProcThunk, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, window_title.data(), nullptr };
            wc.hIcon = LoadIconW(wc.hInstance, MAKEINTRESOURCEW(102));
            ::RegisterClassExW(&wc);
        }
        ~WindowClass()
        {
            ::UnregisterClassW(window_title.data(), wc.hInstance);
        }

    public:
        WNDCLASSEXW wc;
    };

public:
    Window(uint32_t width, uint32_t height);
    ~Window()
    {
        ImGui_ImplWin32_Shutdown();
        ::DestroyWindow(hwnd);
    }

    int PollMessages() noexcept
    {
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT) {
                return 1;
            }
            //  done = true;
            if (msg.message == WM_DESTROY) {
                return 1;
            }
        }
        return 0;
    }

public:
    HWND GetHWND() const noexcept
    {
        return hwnd;
    }
    std::pair<uint32_t, uint32_t> GetSize() const noexcept {
        return {
            width,
            height
        };
    }

private:
    LRESULT               WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT WINAPI WndProcThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
    {
        auto* window = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
        return window->WndProc(hWnd, msg, wParam, lParam);
    }

private:
    WindowClass wclass;
    HWND        hwnd;
    uint32_t    width;
    uint32_t    height;
};
