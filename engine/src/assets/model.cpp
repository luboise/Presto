#include "presto/assets/model.h"

#include "internal/rendering/types.h"

#include <utility>

namespace Pr {
bool ModelAsset::load() { return true; };

ModelAsset::ModelAsset(asset_name_t modelName) : Asset(std::move(modelName)) {}

BoundingBox ModelAsset::getBoundingBox() {
    BoundingBox box{};

    for (const MeshDraw& draw : draws_) {
        // TODO: Fix this to work with the new meshes
        // box.merge(draw.mesh->getBoundingBox());
    }

    return box;
}

ModelAsset& ModelAsset::addMesh(MeshPtr mesh, MaterialPtr material) {
    PR_ASSERT(mesh != nullptr,
              "An added mesh to a model asset must not be null.");

    if (material == nullptr) {
        PR_WARN("A mesh has been added to a ModelAsset without a material.");
    }

    draws_.push_back(MeshDraw{.mesh{mesh}, .material{material}});

    return *this;
};

ModelAsset& ModelAsset::clear() {
    draws_.clear();

    return *this;
}

Pr::size_t ModelAsset::meshCount() const { return this->draws_.size(); };

}  // namespace Pr
