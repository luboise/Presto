#include "OpenGLMaterial.h"

#include "Rendering/Utils/RenderingUtils.h"

namespace Presto {
    OpenGLMaterial::OpenGLMaterial(ShaderPtr& ptr) {
        PR_ASSERT(ptr->ready(),
                  "Unable to create Material from unready shader.");
        shader_ = ptr;
    };

    OpenGLMaterial::OpenGLMaterial(OpenGLShader&& s) {
        PR_ASSERT(s.ready(), "Unable to create Material from unready shader.");
        shader_ = std::make_shared<OpenGLShader>(std::move(s));
    };

    void OpenGLMaterial::bind() {
        shader_->use();
        PR_ASSERT(diffuseTexture_ != nullptr,
                  "Unable to bind material without a diffuse texture.");

        diffuseTexture_->bind(0);
    }

    void OpenGLMaterial::unbind() { glUseProgram(0); }

    void OpenGLMaterial::setProperty(std::string /*property*/,
                                     const void* /*value*/) {};

    ShaderPtr& OpenGLMaterial::getShader() { return shader_; };

    void OpenGLMaterial::setDiffuse(OpenGLTexture* image) {
        diffuseTexture_ = image;
    };
}  // namespace Presto
