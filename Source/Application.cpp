#include "Application.hpp"

#include <glad/glad.h>

#include "Window.hpp"

namespace CatalystZero
{
    Application::Application()
        : m_IsRunning(true) { }
    
    Application::~Application() { }

    void Application::Run()
    {
        Window window(1280, 720, "Catalyst Zero");

        while (m_IsRunning)
        {
            window.ProcessEvents();
            
            Vec2I windowSize = window.GetSize();
            glViewport(0, 0, windowSize.X, windowSize.Y);
            glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            window.SwapBuffers();
        }
    }
}