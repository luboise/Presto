#pragma once

#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {

class PipelineBuilder {
    friend class Renderer;

   public:
    PipelineBuilder& setShader(const AssetPath& shaderPath, ShaderStage type);
    virtual PipelineBuilder& setShader(const char* data, ShaderStage type) = 0;

    virtual PipelineStructure build() = 0;

   protected:
    PipelineBuilder();
};
}  // namespace Presto
