#include "OpenGLMaterial.h"

#include "Rendering/Utils/RenderingUtils.h"

namespace Presto {
    OpenGLMaterial::OpenGLMaterial(OpenGLShader& s) {
        PR_ASSERT(s.ready(), "Unable to create Material from unready shader.");
        shader_ = &s;
    };

    void OpenGLMaterial::bind() {
        ShaderMatrices mats{};

        constexpr glm::float32 FOV_Y = 90;

        mats.view = renderViewMatrix_;

        auto framebufferSize{_glfwWindow->getFramebufferSize()};

        mats.projection = RenderingUtils::getProjectionMatrix(
            FOV_Y, framebufferSize.width, framebufferSize.height);
        mats.transform = transform;

        shader_->setUniform("view", mats.view);
        shader_->setUniform("projection", mats.projection);
        shader_->setUniform("transform", mats.transform);

        // TODO: Make this a one-off function call
        shader_->setUniform("sampler1", std::int8_t(0));

        shader_->use();
    }

    void OpenGLMaterial::setProperty(std::string property, const void* value) {
    };

}  // namespace Presto
