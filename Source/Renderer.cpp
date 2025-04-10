#include "Renderer.hpp"

#include <Windows.h>
#include <glad/glad.h>

namespace CatalystZero
{
    Renderer::Renderer()
    {
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
    
    void Renderer::Update()
    {
        
    }
}