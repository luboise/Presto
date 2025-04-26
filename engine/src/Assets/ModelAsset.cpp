#include <utility>

#include "Presto/Assets/ModelAsset.h"
#include "Presto/Rendering/MeshData.h"

namespace Presto {
bool ModelAsset::load() { return true; };

ModelAsset::ModelAsset(asset_name_t modelName) : Asset(std::move(modelName)) {}

BoundingBox ModelAsset::getBoundingBox() {
    BoundingBox box{};

    for (const MeshDraw& draw : draws_) {
        box.merge(draw.mesh->getBoundingBox());
    }

    return box;
}

ModelAsset& ModelAsset::addMesh(MeshPtr mesh, MaterialPtr material) {
    draws_.push_back(MeshDraw{.mesh{mesh}, .material{material}});

    return *this;
};

ModelAsset& ModelAsset::clear() {
    draws_.clear();

    return *this;
}

Presto::size_t ModelAsset::meshCount() const { return this->a; };

}  // namespace Presto
