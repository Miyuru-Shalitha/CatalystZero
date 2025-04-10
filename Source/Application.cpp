#include "Application.hpp"

#include <Windows.h>
#include <iostream>

#include "Window.hpp"
#include "Renderer.hpp"
#include "ImmediateModeRenderer.hpp"
#include "RenderCommands.hpp"
#include "Timer.hpp"

namespace CatalystZero
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
        : m_IsRunning(true)
    {
        s_Instance = this;
    }
    
    Application::~Application() { }

    void Application::Run()
    {
        Window window(1280, 720, "Catalyst Zero");
        // Renderer renderer;
        ImmediateModeRenderer immediateModeRenderer;

        Timer timer;

        while (m_IsRunning)
        {
            window.ProcessEvents();
            
            std::cout << timer.GetDeltaTime() << "\n";
            
            Vec2I windowSize = window.GetSize();
            RenderCommand::SetViewport(0, 0, windowSize.X, windowSize.Y);
            RenderCommand::SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
            RenderCommand::Clear();
            
            immediateModeRenderer.DrawQuad();

            window.SwapBuffers();

            LARGE_INTEGER currentCounter;
            QueryPerformanceCounter(&currentCounter);

            timer.Tick();
        }
    }
    
    void Application::OnEvent(const Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowClosedEvent>([&](const WindowClosedEvent& event) {
            m_IsRunning = false;
        });
    }
}