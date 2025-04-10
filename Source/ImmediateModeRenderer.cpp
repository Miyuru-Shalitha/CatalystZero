#include "ImmediateModeRenderer.hpp"

#include <Windows.h>
#include <glad/glad.h>
#include <array>

namespace CatalystZero
{
    ImmediateModeRenderer::ImmediateModeRenderer()
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
        
        std::array<float, 12> vertices = {
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f
        };
        
        std::array<uint32_t, 6> indices = {
            0, 1, 2,
            2, 3, 0
        };
        
        glCreateVertexArrays(1, &m_QuadVertexArray);
        glBindVertexArray(m_QuadVertexArray);

        glCreateBuffers(1, &m_QuadVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_QuadVertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        
        glCreateBuffers(1, &m_QuadIndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

        glBindVertexArray(0);
    }
    
    void ImmediateModeRenderer::DrawQuad()
    {
        glBindVertexArray(m_QuadVertexArray);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}