#include "Presto/Rendering/Renderer.h"
#include "Presto/Components/CameraComponent.h"

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
}  // namespace Presto
