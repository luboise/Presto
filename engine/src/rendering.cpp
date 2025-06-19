module presto.internal.rendering;

#include "Rendering/Renderer.h"

#include "Presto/Objects/Components/CameraComponent.h"
#include "Rendering/OpenGL/OpenGLRenderer.h"

#include "Presto/Runtime/GLFWAppWindow.h"

export namespace Presto {
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

Buffer::Buffer(BufferType type, Presto::size_t size)
    : type_(type), size_(size) {};

Presto::size_t Buffer::getWriteSize(Presto::size_t desiredSize,
                                    Presto::size_t startOffset) const {
    Presto::size_t write_size{std::min(desiredSize, this->size_ - startOffset)};

    if (write_size != desiredSize) {
        PR_WARN(
            "Requested write of size {} to Buffer of size {} exceeds the "
            "boundaries "
            "of "
            "the buffer by {} bytes. Writing only up until the end of the "
            "buffer.",
            desiredSize, this->size_, desiredSize - write_size);
    }

    return write_size;
}

Buffer::BufferType Buffer::type() const { return type_; };
Presto::size_t Buffer::size() const { return size_; }

}  // namespace Presto
