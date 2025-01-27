#include "Presto/Rendering/Renderer.h"

namespace Presto {
    void Renderer::setCameraData(Camera& camera) {
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
}  // namespace Presto
