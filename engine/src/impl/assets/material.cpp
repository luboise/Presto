module presto.assets.material;

import presto.internal.managers.rendering_manager;

import std;

namespace Pr {
MaterialAsset::MaterialAsset(Pr::string name, pipeline_id_t pipelineId,
                             const UniformLayout& layout)
    : Asset(std::move(name)), pipelineId_(pipelineId) {
    // TODO: Put validation checks here before overwriting
    this->uniformLayout_ = layout;
}

UniformLayout MaterialAsset::uniformLayout() const { return uniformLayout_; }

bool MaterialAsset::load() { return true; };

/*
Ptr<MaterialInstance> MaterialAsset::createInstance() {
    return std::make_shared<MaterialInstance>(shared_from_this());
};
*/

}  // namespace Pr
