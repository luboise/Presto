#include "PrestoCore/GLFWAppWindow.h"

#include "PrestoCore/Rendering/OpenGLRenderer.h"
#include "PrestoCore/Rendering/RendererFactory.h"

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
