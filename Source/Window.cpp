#include "Window.hpp"

#include <Windows.h>
#include <glad/glad.h>
#include <wglext.h>

namespace CatalystZero
{
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT result = { };

        switch (uMsg)
        {
            default:
            {
                result = DefWindowProcA(hwnd, uMsg, wParam, lParam);
            } break;
        }

        return result;
    }

    Window::Window(int width, int height, const char* title)
    {
        HINSTANCE hInstance = GetModuleHandleA(nullptr);
        
        WNDCLASSA wc = { };
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = "Windows_Window_Class_Name";
        
        if (!RegisterClassA(&wc))
        {
            // TODO(Miyuru): Log.
        }
        
        HWND fakeWindow = CreateWindowExA(
            0,
            wc.lpszClassName,
            "Fake_Window",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            0, 0,
            nullptr,
            nullptr,
            hInstance,
            nullptr
        );
        
        HDC fakeDeviceContext = GetDC(fakeWindow);
        
        PIXELFORMATDESCRIPTOR fakePixelFormatDescriptor = { };
        fakePixelFormatDescriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        fakePixelFormatDescriptor.nVersion = 1;
        fakePixelFormatDescriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        fakePixelFormatDescriptor.cColorBits = 32;
        fakePixelFormatDescriptor.cAlphaBits = 8;
        fakePixelFormatDescriptor.cDepthBits = 24;
        
        int fakePixelFormatDescroptorID = ChoosePixelFormat(fakeDeviceContext, &fakePixelFormatDescriptor);
        
        if (!fakePixelFormatDescroptorID)
        {
            // TODO(Miyuru): Log.
        }
        
        BOOL setPixelFormatStatus = SetPixelFormat(fakeDeviceContext, fakePixelFormatDescroptorID, &fakePixelFormatDescriptor);
        
        if (setPixelFormatStatus == FALSE)
        {
            // TODO(Miyuru): Log.
        }
        
        HGLRC fakeRenderingContext = wglCreateContext(fakeDeviceContext);

        if (!fakeRenderingContext)
        {
            // TODO(Miyuru): Log.
        }
        
        BOOL wglMakeCurrentStatus = wglMakeCurrent(fakeDeviceContext, fakeRenderingContext);
        
        if (wglMakeCurrentStatus == FALSE)
        {
            // TODO(Miyuru): Log.
        }

        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
        
        if (!wglChoosePixelFormatARB)
        {
            // TODO(Miyuru): Log.
        }
        
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));

        if (!wglCreateContextAttribsARB)
        {
            // TODO(Miyuru): Log.
        }

        m_Handle = CreateWindowExA(
            0,
            wc.lpszClassName,
            title,
            WS_VISIBLE | WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            width, height,
            nullptr,
            nullptr,
            hInstance,
            nullptr
        );
        
        if (!m_Handle)
        {
            // TODO(Miyuru): Log.
        }

        HDC deviceContext = GetDC(static_cast<HWND>(m_Handle));
        
        if (!deviceContext)
        {
            // TODO(Miyuru): Log.
        }
        
        const int pixelFormatAttributes[] = {
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
            WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
            WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
            WGL_COLOR_BITS_ARB, 32,
            WGL_ALPHA_BITS_ARB, 8,
            WGL_DEPTH_BITS_ARB, 24,
            WGL_STENCIL_BITS_ARB, 8,
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB, 4,
            0
        };
        
        int pixelFormatID = 0;
        UINT numFormats = 0;
        bool wglChoosePixelFormatARBStatus = wglChoosePixelFormatARB(deviceContext, pixelFormatAttributes, nullptr, 1, &pixelFormatID, &numFormats);
        
        if (!wglChoosePixelFormatARBStatus && !numFormats)
        {
            // TODO(Miyuru): Log.
        }
        
        PIXELFORMATDESCRIPTOR pixelFormatDescriptor = { };
        int describePixelFormatStatus = DescribePixelFormat(deviceContext, pixelFormatID, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDescriptor);
        
        if (!describePixelFormatStatus)
        {
            // TODO(Miyuru): Log.
        }
        
        setPixelFormatStatus = SetPixelFormat(deviceContext, pixelFormatID, &pixelFormatDescriptor);
        
        if (!setPixelFormatStatus)
        {
            // TODO(Miyuru): Log.
        }
        
        int contextAttributes[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 6,
            WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0
        };
        
        HGLRC renderingContext = wglCreateContextAttribsARB(deviceContext, 0, contextAttributes);
        
        if (!renderingContext)
        {
            // TODO(Miyuru): Log.   
        }
        
        wglMakeCurrentStatus = wglMakeCurrent(nullptr, nullptr);
        
        if (wglMakeCurrentStatus == FALSE)
        {
            // TODO(Miyuru): Log.
        }

        BOOL wglDeleteContextStatus = wglDeleteContext(fakeRenderingContext);
        
        if (wglDeleteContextStatus == FALSE)
        {
            // TODO(Miyuru): Log.
        }

        int releaseDeviceContextStatus = ReleaseDC(fakeWindow, fakeDeviceContext);
        
        if (releaseDeviceContextStatus == FALSE)
        {
            // TODO(Miyuru): Log.
        }
        
        BOOL destroyWindowStatus = DestroyWindow(fakeWindow);
        
        if (destroyWindowStatus == FALSE)
        {
            // TODO(Miyuru): Log.
        }

        wglMakeCurrentStatus = wglMakeCurrent(deviceContext, renderingContext);
        
        if (wglMakeCurrentStatus == FALSE)
        {
            // TODO(Miyuru): Log.
        }

        if (!gladLoadGLLoader(static_cast<GLADloadproc>([](const char* name) -> void* {
            void* p = static_cast<void*>(wglGetProcAddress(name));
            
            if (p == 0 ||
               (p == reinterpret_cast<void*>(0x1)) || (p == reinterpret_cast<void*>(0x2) || (p == reinterpret_cast<void*>(0x3)) ||
               (p == reinterpret_cast<void*>(-1)))
            )
            {
                HMODULE module = LoadLibraryA("opengl32.dll");
                p = GetProcAddress(module, name);
            }
            
            return p;
        })))
        {
            // TODO(Miyuru): Log.
        }
    }

    Window::~Window()
    {
        
    }
    
    void Window::ProcessEvents() const
    {
        MSG message = { };
        
        while (PeekMessageA(&message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
    
    // This is to avoid the confution of 'SwapBuffers' with windows 'SwapBuffers' function.
    static void SwapBuffers_(HDC deviceContext)
    {
        SwapBuffers(deviceContext);
    }

    void Window::SwapBuffers() const
    {
        HDC deviceContext = GetDC(static_cast<HWND>(m_Handle));
        SwapBuffers_(deviceContext);
    }
    
    Vec2I Window::GetSize() const
    {
        RECT rect = { };
        BOOL getClientRectStatus = GetClientRect(static_cast<HWND>(m_Handle), &rect);
        
        if (!getClientRectStatus)
        {
            // TODO(Miyuru): Log.
        }
        
        return { rect.right - rect.left, rect.bottom - rect.top };
    }
}