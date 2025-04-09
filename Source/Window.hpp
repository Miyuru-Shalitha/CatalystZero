#pragma once

// This struct is temporary and it should be defined in like a 'Math.hpp' file.
struct Vec2I
{
    int X;
    int Y;
};

namespace CatalystZero
{
    class Window
    {
    public:
        Window(int width, int height, const char* title);
        Window(const Window& other) = delete;
        Window(Window&& other) = delete;
        ~Window();
        
        void ProcessEvents() const;
        void SwapBuffers() const;
        
        Vec2I GetSize() const;
        
    private:
        void* m_Handle;
    };
}