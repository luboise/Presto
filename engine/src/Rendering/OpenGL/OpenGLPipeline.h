#pragma once

#include "Presto/Rendering/Pipeline.h"
#include "Rendering/OpenGL/OpenGLShader.h"

#include "OpenGLDrawManager/OpenGLTexture.h"

namespace Presto {
    using ShaderPtr = OpenGLShader::opengl_shader_ptr_t;

    class OpenGLPipeline : public Pipeline {
        friend class OpenGLRenderer;

        explicit OpenGLPipeline(ShaderPtr& ptr);
        explicit OpenGLPipeline(OpenGLShader&& s);

       public:
        void setProperty(std::string property, const void* value) override;
        void setDiffuse(OpenGLTexture*);

       private:
        void bind() override;
        void unbind() override;

        OpenGLTexture* diffuseTexture_{nullptr};

        ShaderPtr shader_;
        ShaderPtr& getShader();
    };
}  // namespace Presto
