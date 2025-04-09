#pragma once

#include "EventSystem.hpp"

namespace CatalystZero
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();
        void OnEvent(const Event& event);
        
        inline static Application& GetInstance() { return *s_Instance; }
        
    private:
        bool m_IsRunning;
        
        static Application* s_Instance;
    };
}