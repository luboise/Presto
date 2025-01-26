#pragma once

#include "Presto/Rendering/Material.h"
#include "Rendering/OpenGL/OpenGLShader.h"

namespace Presto {
    class OpenGLMaterial : public Material {
        OpenGLMaterial(OpenGLShader s);
    };
}  // namespace Presto
