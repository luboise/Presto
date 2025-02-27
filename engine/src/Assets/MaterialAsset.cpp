#include <algorithm>
#include <utility>

#include "Presto/Assets/MaterialAsset.h"

#include "Modules/RenderingManager.h"
#include "Presto/Assets/ImportTypes.h"
#include "Presto/Core/Constants.h"
#include "Presto/Rendering/PipelineTypes.h"

namespace Presto {
MaterialAsset::MaterialAsset(PR_STRING_ID name, const PipelineStructure& ps)
    : Asset(std::move(name)), pipelineId_(ps.pipeline_id) {
    auto& rm{RenderingManager::get()};

    MaterialStructure ms{materialStructureFromPipeline(ps)};

    // TODO: Put validation checks here before appending the structure
    this->structure_ = ms;

    // Allocate a buffer for each uniform block (needed for hotswap)
    for (const auto& uniform_block : ms.blocks) {
        auto buffer{rm.createUniformBuffer(uniform_block.size())};
    }

    std::vector<MaterialProperty> properties;

    this->properties_ = properties;
}

MaterialStructure MaterialAsset::getStructure() const { return structure_; }

bool MaterialAsset::load() {
    static_assert(false);
    return true;
};

MaterialProperty* MaterialAsset::getProperty(const PR_STRING_ID& name) {
    if (const auto found{
            std::ranges::find_if(properties_,
                                 [name](const MaterialProperty& prop) {
                                     return prop.name == name;
                                 })};
        found != properties_.end()) {
        return &*found;
    }

    return nullptr;
}

MaterialStructure MaterialAsset::materialStructureFromPipeline(
    const PipelineStructure& ps) {
    static_assert(false);
    MaterialStructure ms{};
    return ms;
};

Ptr<MaterialInstance> MaterialAsset::createInstance() {
    return std::make_shared<MaterialInstance>(shared_from_this());
};

pipeline_id_t MaterialAsset::pipelineId() const { return pipelineId_; }
}  // namespace Presto
