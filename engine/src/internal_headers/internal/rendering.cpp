#include "internal/rendering.h"

#include "./base.h"
#include "internal/glfw.h"
#include "internal/rendering/opengl.h"
#include "presto/objects/components.h"

namespace Pr {
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
            return Allocated<Renderer>{new OpenGLRenderer(window)};
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

void Renderer::setAspectRatio(aspect_ratio_t ratio) {
    aspectRatio_ = ratio;
    this->recalculateViewport();
};

void Renderer::setExtents(VisualExtents extents) {
    extents_ = extents;
    this->recalculateViewport();
};

void Renderer::setWindow(GLFWAppWindow* window) {
    this->_glfwWindow = window;
    this->setExtents(window->framebufferSize());
}

}  // namespace Pr
