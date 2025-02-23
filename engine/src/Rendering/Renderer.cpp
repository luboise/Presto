#include "Rendering/Renderer.h"

#include "Presto/Components/CameraComponent.h"
#include "Rendering/OpenGL/OpenGLRenderer.h"

#include "Presto/Runtime/GLFWAppWindow.h"

#include <memory>

namespace Presto {
void Renderer::setCameraData(CameraComponent& camera) {
    globalUniforms_ = {.view = camera.getViewMatrix(),
                       .projection = camera.getProjectionMatrix()};
    setDirty();
}

void Renderer::setCameraData(GlobalUniforms&& uniforms) {
    globalUniforms_ = uniforms;
    setDirty();
};

void Renderer::setObjectData(ObjectUniforms&& uniforms) {
    objectUniforms_ = uniforms;
    setDirty();
};

Allocated<Renderer> Renderer::create(RENDER_LIBRARY lib,
                                     GLFWAppWindow* window) {
    switch (lib) {
        case OPENGL: {
            Allocated<OpenGLRenderer> renderer{
                std::make_unique<OpenGLRenderer>(window)};

            Allocated<Renderer> bruh = std::move(renderer);

            return renderer;
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

}  // namespace Presto
