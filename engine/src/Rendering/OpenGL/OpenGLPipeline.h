#pragma once

#include "Presto/Rendering/Pipeline.h"
#include "Presto/Rendering/RenderTypes.h"

#include "OpenGLDrawManager/OpenGLTexture.h"
#include "Rendering/OpenGL/OpenGLShader.h"

#include "OpenGLTypes.h"

namespace Presto {
using ShaderPtr = OpenGLShader::opengl_shader_ptr_t;

class OpenGLPipeline : public Pipeline {
    friend class OpenGLRenderer;
    friend class OpenGLDrawManager;

    explicit OpenGLPipeline(ShaderPtr& ptr);
    explicit OpenGLPipeline(OpenGLShader&& s);

   public:
    void setProperty(std::string property, const void* value) override;
    void setDiffuse(OpenGLTexture*);

    [[nodiscard]] const AttributeSet& getAttributes() const {
        return attributeSet_;
    };

   private:
    void bind() override;
    void unbind() override;

    OpenGLTexture* diffuseTexture_{nullptr};
    renderer_pipeline_id_t id_{UNREGISTERED_RENDER_DATA_ID};

    AttributeSet attributeSet_;

    ShaderPtr shader_;
    ShaderPtr& getShader();
};
}  // namespace Presto
