#pragma once

#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Types/AssetTypes.h"

namespace Presto {

class PipelineBuilder {
    friend class Renderer;

   public:
    PipelineBuilder& setShader(const AssetArg& shaderPath, ShaderStage type);
    virtual PipelineBuilder& setShader(const char* data, ShaderStage type) = 0;

    virtual PipelineStructure build() = 0;

    virtual ~PipelineBuilder() = default;

    PipelineBuilder(const PipelineBuilder&) = delete;
    PipelineBuilder(PipelineBuilder&&) = delete;
    PipelineBuilder& operator=(const PipelineBuilder&) = delete;
    PipelineBuilder& operator=(PipelineBuilder&&) = delete;

   protected:
    PipelineBuilder() = default;
};
}  // namespace Presto
