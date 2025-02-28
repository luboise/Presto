#include <utility>

#include "Presto/Assets/MaterialAsset.h"

#include "Modules/RenderingManager.h"
#include "Presto/Rendering/PipelineTypes.h"

namespace Presto {
MaterialAsset::MaterialAsset(PR_STRING_ID name, const PipelineStructure& ps)
    : Asset(std::move(name)), pipelineId_(ps.pipeline_id) {
    UniformLayout layout{createLayoutFromPipelineStructure(ps)};

    // TODO: Put validation checks here before overwriting
    this->uniformLayout_ = layout;
}

UniformLayout MaterialAsset::uniformLayout() const { return uniformLayout_; }

bool MaterialAsset::load() { return true; };

UniformLayout MaterialAsset::createLayoutFromPipelineStructure(
    const PipelineStructure& ps) {
    static_assert(false);
    UniformLayout layout{};
    return layout;
};

Ptr<MaterialInstance> MaterialAsset::createInstance() {
    return std::make_shared<MaterialInstance>(shared_from_this());
};

}  // namespace Presto
