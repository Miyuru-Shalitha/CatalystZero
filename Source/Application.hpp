#pragma once

namespace CatalystZero
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();
        
    private:
        bool m_IsRunning;
    };
}