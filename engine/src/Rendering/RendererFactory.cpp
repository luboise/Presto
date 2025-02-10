#include "Presto/Rendering/RendererFactory.h"
#include "Rendering/OpenGL/OpenGLRenderer.h"

// DO NOT MOVE! Need to import gl.h before glew.h

#include "Presto/Runtime/GLFWAppWindow.h"

using Presto::Renderer;

Allocated<Renderer> Presto::CreateRenderer(RENDER_LIBRARY lib,
                                           GLFWAppWindow* window) {
    switch (lib) {
        case OPENGL: {
            return std::make_unique<OpenGLRenderer>(window);
        }
            /*
case VULKAN:
_renderer = new VulkanRenderer(window);
break;
            */
        default:
            throw std::runtime_error("Invalid render library specified.");
    }
}
