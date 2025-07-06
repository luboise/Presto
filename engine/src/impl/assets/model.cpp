module presto.assets.model;

import std;

import presto.core.logging;

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

ModelAsset& ModelAsset::addMesh(Pr::Ptr<Pr::MeshAsset> mesh,
                                Pr::Ptr<Pr::MaterialInstance> material) {
    Pr::Assert(mesh != nullptr,
               "An added mesh to a model asset must not be null.");

    if (material == nullptr) {
        Pr::CoreLog(
            WARN, "A mesh has been added to a ModelAsset without a material.");
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
