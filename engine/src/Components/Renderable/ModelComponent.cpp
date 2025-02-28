#include "Presto/Objects/Components/Renderable/ModelComponent.h"

namespace Presto {

ModelComponent::ModelComponent(const ModelPtr& modelAsset) {
    for (const auto& mesh : modelAsset->getMeshes()) {
        meshes_.push_back(mesh);
        materialOverrides_.push_back(nullptr);
    }
};

void ModelComponent::onEnterScene() {
    for (auto& mesh : meshes_) {
        mesh->ensureLoaded();
    }

    // This might be needed later if selective material usage is implemented
    /*
for (auto& material : materialOverrides_) {
    if (material != nullptr) {
        material->ensureLoaded();
    }
}
    */
};
}  // namespace Presto
