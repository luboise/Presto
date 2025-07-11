module presto.assets.mesh;

import std;

import presto.internal.managers.rendering_manager;
import presto.internal;

import presto.assets.image;

import presto.internal.rendering;

import presto.types.core;

namespace Pr {

struct MeshAsset::Impl {
    MeshData mesh_data;

    Pr::Ptr<Pr::MaterialInstance> default_material;

    Ptr<Mesh> mesh;

    BoundingBox box;
};

MeshAsset::MeshAsset() { impl_ = std::make_unique<Impl>(); };
MeshAsset::~MeshAsset() = default;

bool MeshAsset::load() {
    this->impl_->mesh_data.pipeline_id = PR_PIPELINE_DEFAULT_3D;

    this->impl_->mesh = RenderingManager::get().loadMesh(impl_->mesh_data);
    return impl_->mesh->registrationId() != PR_UNREGISTERED;
}

MeshAsset& MeshAsset::setDefaultMaterial(
    const Pr::Ptr<Pr::MaterialInstance>& material) {
    if (!modifiable()) {
        return *this;
    }

    impl_->default_material = material;
    return *this;
};
//
// TODO: Implement bounding box logic on import of mesh
BoundingBox MeshAsset::getBoundingBox() const { return impl_->box; }

bool MeshAsset::modifiable() const {
    if (this->loaded()) {
        Pr::Log(
            ERROR,
            "Unable to modify MeshAsset \"{}\" while it is loaded. It must be "
            "unloaded before being modified.\nSkipping this modification.",
            this->name());
        return false;
    }

    return true;
};

Pr::Ptr<Pr::MaterialInstance>& MeshAsset::defaultMaterial() const {
    return impl_->default_material;
};

MeshAsset& MeshAsset::setMeshData(MeshData data) {
    impl_->mesh_data = std::move(data);
    return *this;
};

void BoundingBox::merge(const BoundingBox& other) {
    this->x_min = std::min(x_min, other.x_min);
    this->y_min = std::min(y_min, other.y_min);
    this->z_min = std::min(z_min, other.z_min);

    this->x_max = std::min(x_max, other.x_max);
    this->y_max = std::min(y_max, other.y_max);
    this->z_max = std::min(z_max, other.z_max);
}

double BoundingBox::getNormalisingFactor() const {
    double new_val{0};

    double max{x_min};

    new_val = std::abs(x_max);
    max = std::max(new_val, max);

    new_val = std::abs(y_min);
    max = std::max(new_val, max);

    new_val = std::abs(y_max);
    max = std::max(new_val, max);

    new_val = std::abs(z_min);
    max = std::max(new_val, max);

    new_val = std::abs(z_max);
    max = std::max(new_val, max);

    if (max == 0) {
        return 0;
    }

    return 0.5 / max;
}

}  // namespace Pr
