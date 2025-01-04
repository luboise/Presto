#include "PrestoCore/Rendering/RendererFactory.h"
#include "Rendering/OpenGL/OpenGLRenderer.h"

// DO NOT MOVE! Need to import gl.h before glew.h

#include "PrestoCore/GLFWAppWindow.h"
using Presto::Renderer;

Renderer* Presto::CreateRenderer(RENDER_LIBRARY lib, GLFWAppWindow* window) {
    switch (lib) {
        case OPENGL: {
            auto* new_renderer{new OpenGLRenderer(window)};
            return (Renderer*)new_renderer;
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
