module presto.assets.material;

import presto.internal.managers.rendering_manager;

import std;

namespace Pr {

MaterialAsset::MaterialAsset(Pr::string name, pipeline_id_t pipelineId,
                             const UniformLayout& layout)
    : Asset(std::move(name)), pipelineId_(pipelineId), uniformLayout_(layout) {
    // TODO: Put validation checks here before overwriting
}

UniformLayout MaterialAsset::uniformLayout() const { return uniformLayout_; }

bool MaterialAsset::load() { return true; };

/*
Ptr<MaterialInstance> MaterialAsset::createInstance() {
    return std::make_shared<MaterialInstance>(shared_from_this());
};
*/

}  // namespace Pr
