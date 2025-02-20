#include "Presto/Rendering/PipelineBuilder.h"

#include "Presto/Assets/AssetPath.h"
#include "Presto/Utils/File.h"

namespace Presto {

PipelineBuilder& PipelineBuilder::setShader(const AssetArg& shaderPath,
                                            ShaderStage type) {
    auto data = Utils::File::ReadFile(shaderPath);
    return setShader(data.data(), type);
};

}  // namespace Presto
