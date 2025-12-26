#include "window.hpp"

Window::Window(uint32_t width, uint32_t height)
    : width(width)
    , height(height)
{
    hwnd = ::CreateWindowW(
            wclass.wc.lpszClassName,
            WindowClass::window_title.data(),
            WS_OVERLAPPEDWINDOW | WS_EX_TOOLWINDOW | WS_EX_NOPARENTNOTIFY,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            width,
            height,
            nullptr,
            nullptr,
            wclass.wc.hInstance,
            nullptr);
    ::SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_SIZEBOX);
    ::SetWindowLongPtrW(hwnd, GWLP_USERDATA, LONG_PTR(this));

    //
    IMGUI_CHECKVERSION();
    //
    ImGui::CreateContext();

    ImGui_ImplWin32_InitForOpenGL(hwnd);

    ::AnimateWindow(hwnd, 100, AW_BLEND);
    ::UpdateWindow(hwnd);
}

LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }

    switch (msg) {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED) {
            width  = LOWORD(lParam);
            height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) { // Disable ALT application menu
            return 0;
        }
        break;
    case WM_DESTROY:
        exit(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
