#pragma once

#include "Presto/Rendering/Material.h"
#include "Rendering/OpenGL/OpenGLShader.h"

namespace Presto {
    class OpenGLMaterial : public Material {
        friend class OpenGLRenderer;

        OpenGLMaterial(OpenGLShader& s);

       public:
        void setProperty(std::string property, const void* value) override;

       private:
        void bind() override;
        void unbind() override;

        OpenGLShader* shader_;
    };
}  // namespace Presto
