#include "Presto/Components/Renderable/ModelComponent.h"

namespace Presto {
    /*
        void Model::setMesh(ModelAsset& resource) {
            if (!resource.loaded()) {
                RenderingManager::get().loadMeshOnGpu(resource);
            }

            meshResource_ = &resource;
        }
            */

    // bool Model::hasResource() const { return meshResource_ != nullptr; }

    // const ModelAsset& getMesh() { return *meshResource_; }

    ModelComponent::ModelComponent(const ModelPtr& modelAsset) {
        for (const auto& mesh : modelAsset->getMeshes()) {
            meshes_.push_back(mesh);
            materialOverrides_.push_back(nullptr);
        }
    };
}  // namespace Presto
