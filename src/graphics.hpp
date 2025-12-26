#pragma once
#include "imgui_impl_opengl3.h"

class Graphics
{
public:
    Graphics(HWND hwnd)
        : hwnd(hwnd)
    {
        CreateDeviceWGL(hwnd);
        wglMakeCurrent(hDC, hRC);

        if (!ImGui_ImplOpenGL3_Init()) {
            debug_log("[Fatal Error] OGL3 NOT SUPPORT ON YOU GPU");
        } else {
            debug_log("(OGL3)::InitToWIN32 OK!");
        }
    }
    ~Graphics()
    {
        ImGui_ImplOpenGL3_Shutdown();
        CleanupDeviceWGL();
        wglDeleteContext(hRC);
    }

public:
    void Present()
    {
        ::SwapBuffers(hDC);
    }

private:
    bool CreateDeviceWGL(HWND hWnd)
    {
        hDC                       = ::GetDC(hWnd);
        PIXELFORMATDESCRIPTOR pfd = {
            .nSize      = sizeof(pfd),
            .nVersion   = 3,
            .dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
            .iPixelType = PFD_TYPE_RGBA,
            .cColorBits = 32,
            .cAlphaBits = 8,
        };

        const int pf = ::ChoosePixelFormat(hDC, &pfd);
        if (pf == 0) {
            return false;
        }
        if (!::SetPixelFormat(hDC, pf, &pfd)) {
            return false;
        }
        hRC = ImGuiCreateDevice(hDC, 3, 3);
        return true;
    }
    void CleanupDeviceWGL()
    {
        wglMakeCurrent(nullptr, nullptr);
        ::ReleaseDC(hwnd, hDC);
    }

private:
    HDC   hDC;
    HGLRC hRC;
    HWND  hwnd;
};