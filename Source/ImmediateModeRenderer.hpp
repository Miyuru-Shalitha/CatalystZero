#pragma once

#include <cstdint>

namespace CatalystZero
{
    class ImmediateModeRenderer
    {
    public:
        ImmediateModeRenderer();
        
        void DrawQuad();
        
    private:
        uint32_t m_QuadVertexArray;
        uint32_t m_QuadVertexBuffer;
        uint32_t m_QuadIndexBuffer;
    };
}