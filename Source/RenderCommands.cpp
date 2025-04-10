#include "RenderCommands.hpp"

#include <glad/glad.h>

namespace CatalystZero
{
    void RenderCommand::SetViewport(int x, int y, int width, int height)
    {
        glViewport(x, y, width, height);
    }
    
    void RenderCommand::SetClearColor(float r, float g, float b, float a)
    {
        glClearColor(r, g, b, a);
    }
    
    void RenderCommand::Clear()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}