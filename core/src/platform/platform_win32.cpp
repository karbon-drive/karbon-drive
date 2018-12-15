#ifdef _WIN32
#include <Windows.h>
#include <assert.h>
#include "platform.h"


#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "opengl32.lib")


static void
internal_destroy_window(
        struct kci_platform *ctx)
{
        if (ctx->hwnd && ctx->dc) {
                ReleaseDC((HWND)ctx->hwnd, (HDC)ctx->dc);
                ctx->dc = 0;
        }

        if (ctx->hwnd) {
                DestroyWindow((HWND)ctx->hwnd);
                ctx->hwnd = 0;
        }
}


LRESULT CALLBACK
internal_wnd_proc(HWND hWnd, UINT u_msg, WPARAM  w_param, LPARAM  l_param)
{
        LRESULT result = 0;

        struct kci_platform *ctx = (kci_platform*)GetPropA(hWnd, "Karbon");

        switch (u_msg) {
        case WM_CLOSE:
        case WM_QUIT:
        case WM_DESTROY: {
                if (ctx && ctx->hwnd) {
                        internal_destroy_window(ctx);
                }
                break;
        }
        case WM_SIZING: {
                int i = 0;
                break;
        }
        case WM_SIZE: {
                if (ctx) {

                }
                break;
        }
        case WM_MOUSEMOVE: {
                if (ctx) {

                }
                break;
        }
        case WM_MOUSEWHEEL: {
                if (ctx) {

                }
                break;
        }
        case WM_LBUTTONDOWN: {
                if (ctx) {
                        
                }
                break;
        }
        case WM_RBUTTONDOWN: {
                if (ctx) {
                        
                }
                break;
        }
        case WM_MBUTTONDOWN: {
                if (ctx) {
                        
                }
                break;
        }
        case WM_LBUTTONUP: {
                if (ctx) {
                        
                }
                break;
        }
        case WM_RBUTTONUP: {
                if (ctx) {
                        
                }
                break;
        }
        case WM_MBUTTONUP: {
                if (ctx) {
                        
                }
                break;
        }
        case WM_MOVING: {
                int i = 0;
                break;
        }

        default: {
                result = DefWindowProcA(hWnd, u_msg, w_param, l_param);
                break;
        }
        }

        return result;
}


int
kci_platform_setup(
        struct kci_platform *ctx)
{
        /* register class */
        WNDCLASSA wc;
        memset(&wc, 0, sizeof(wc));

        wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wc.lpfnWndProc = (WNDPROC)internal_wnd_proc;
        wc.hInstance = (HINSTANCE)ctx->h_instance;
        wc.lpszClassName = "Karbon";
        wc.hCursor = LoadCursorA(0, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

        if (!RegisterClass(&wc)) {
                return 0;
        }

        DWORD dw_ex_style = WS_EX_ACCEPTFILES;
        DWORD dw_style =
                WS_OVERLAPPEDWINDOW |
                WS_CLIPCHILDREN |
                WS_CLIPSIBLINGS;

        RECT rect;
        rect.left = rect.top = 0;

        ctx->width = 720;
        ctx->height = 480;

        rect.right = ctx->width;
        rect.bottom = ctx->height;
        AdjustWindowRectEx(&rect, dw_style, FALSE, dw_ex_style);

        /* create window */
        HWND hwnd = CreateWindowExA(
                dw_ex_style,
                wc.lpszClassName,
                "KarbonDrive",
                dw_style,
                0, 0,
                rect.right - rect.left,
                rect.bottom - rect.top,
                NULL,
                NULL,
                (HINSTANCE)ctx->h_instance,
                NULL);

        if (!hwnd) {
                assert(!"KD - Failed to create Window");
                return 0;
        }

        HDC dc = GetDC(hwnd);

        /* create gl */
        PIXELFORMATDESCRIPTOR pfd{};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;

        INT pfi = ChoosePixelFormat(dc, &pfd);

        if (pfi == 0) {
                assert(!"KD - Failed to create Pixel format for OGL");
                return 0;
        }

        BOOL pfs = SetPixelFormat(dc, pfi, &pfd);

        if (pfs = FALSE) {
                assert(!"KD - Failed to set Pixel format for OGL");
                return 0;
        }

        HGLRC glrc = wglCreateContext(dc);

        if (!glrc) {
                assert(!"KD - Failed to create OGL context");
                return 0;
        }

        BOOL curr = wglMakeCurrent(dc, glrc);

        if (curr == FALSE) {
                assert(!"KD - Failed to make current");
                return 0;
        }

        /* save info */
        ctx->hwnd = (void*)hwnd;
        ctx->dc = (void*)dc;
        ctx->glrc = (void*)glrc;

        /* display window */
        ShowWindow((HWND)ctx->hwnd, SW_SHOW);
        SetForegroundWindow((HWND)ctx->hwnd);
        SetFocus((HWND)ctx->hwnd);
        SetPropA((HWND)ctx->hwnd, "Karbon", ctx);
}


int
kci_platform_process(
        struct kci_platform *plat)
{
        /* process messages */
        MSG msg;

        /* handle messages we care about! */
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
                switch (msg.message) {
                default: {
                        TranslateMessage(&msg);
                        DispatchMessageA(&msg);
                        break;
                }
                }
        }

        SwapBuffers((HDC)plat->dc);

        return plat->hwnd ? 1 : 0;
}


/* --------------------------------------------------------------- Open GL -- */


int
kci_platform_opengl_make_current(
        struct kci_platform *plat)
{
        BOOL curr = wglMakeCurrent((HDC)plat->dc, (HGLRC)plat->glrc);
        return curr == TRUE ? 1 : 0;
}

#endif
