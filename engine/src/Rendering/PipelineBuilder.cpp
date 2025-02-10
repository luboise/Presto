#include "Presto/Rendering/PipelineBuilder.h"

#include "Presto/Utils/File.h"

namespace Presto {

PipelineBuilder& PipelineBuilder::setShader(const AssetPath& shaderPath,
                                            ShaderStage type) {
    auto data = Utils::File::ReadFile(shaderPath);
    return setShader(data.data(), type);
};

}  // namespace Presto
