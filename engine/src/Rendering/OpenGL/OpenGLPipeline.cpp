#include "OpenGLPipeline.h"

#include "Rendering/Utils/RenderingUtils.h"

namespace Presto {
OpenGLPipeline::OpenGLPipeline(ShaderPtr& ptr) {
    PR_ASSERT(ptr->ready(), "Unable to create Material from unready shader.");
    shader_ = ptr;
};

OpenGLPipeline::OpenGLPipeline(OpenGLShader&& s) {
    PR_ASSERT(s.ready(), "Unable to create Material from unready shader.");
    shader_ = std::make_shared<OpenGLShader>(std::move(s));
};

void OpenGLPipeline::bind() {
    shader_->use();
    PR_ASSERT(diffuseTexture_ != nullptr,
              "Unable to bind material without a diffuse texture.");

    diffuseTexture_->bind(0);
}

void OpenGLPipeline::unbind() { glUseProgram(0); }

void OpenGLPipeline::setProperty(std::string /*property*/,
                                 const void* /*value*/) {};

ShaderPtr& OpenGLPipeline::getShader() { return shader_; };

void OpenGLPipeline::setDiffuse(OpenGLTexture* image) {
    diffuseTexture_ = image;
};
}  // namespace Presto
