#include "Presto/Assets/MeshAsset.h"

#include "Modules/RenderingManager.h"
#include "Presto/Core/Constants.h"
#include "Presto/Rendering/MeshData.h"
#include "Presto/Types/CoreTypes.h"

#include "Rendering/VertexProcessing.h"

namespace Presto {

struct MeshAsset::Impl {
    MeshData mesh_data;

    MaterialPtr default_material;

    BoundingBox box;
};

MeshAsset::MeshAsset() { impl_ = std::make_unique<Impl>(); };
MeshAsset::~MeshAsset() = default;

bool MeshAsset::load() {
    this->impl_->mesh_data.pipeline_id = PR_PIPELINE_DEFAULT_3D;

    impl_->registration_id = RenderingManager::get().loadMesh(impl_->mesh_data);
    return impl_->registration_id != PR_UNREGISTERED;
}

MeshAsset& MeshAsset::setDefaultMaterial(const MaterialPtr& material) {
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
        PR_ERROR(
            "Unable to modify MeshAsset \"{}\" while it is loaded. It must be "
            "unloaded before being modified.\nSkipping this modification.",
            this->name());
        return false;
    }

    return true;
};

MaterialPtr& MeshAsset::defaultMaterial() const {
    return impl_->default_material;
};

MeshAsset& MeshAsset::setMeshData(MeshData data) {
    impl_->mesh_data = std::move(data);
    return *this;
};

/*
MeshAsset& MeshAsset::setVertices(const ImportedAttributeList& attributes) {
    if (!modifiable()) {
        return *this;
    }

    auto processed{processVertices<Vertex3D>(attributes)};

    // TODO: Put checks here to make sure the processed vertices are well formed
    this->impl_->mesh_data.vertices = std::move(processed);

    // TODO: Implement actual bounding box
    impl_->box = {.x_min = -1,
                  .x_max = 1,
                  .y_min = -1,
                  .y_max = 1,
                  .z_min = -1,
                  .z_max = 1};

    return *this;
};

MeshAsset& MeshAsset::setIndices(IndexList indices) {
    this->impl_->mesh_data.indices = std::move(indices);
    return *this;
};

MeshAsset& MeshAsset::setDrawMode(MeshDrawMode mode) {
    impl_->mesh_data.draw_mode = mode;
    return *this;
};



MeshPtr MeshAsset::from(const ImportedMesh& i_mesh) {
    MeshPtr mesh{std::make_shared<MeshAsset>()};

    MeshData data{};

    data.draw_mode = imported_mesh.draw_mode;
    data.setVertices(imported_mesh.attributes);
    data.indices = imported_mesh.indices;

    mesh->setMeshData(std::move(data));

    // Make sure the material is loaded
    if (i_mesh.hasMaterial() && i_mesh.material_data != nullptr) {
        mesh->setDefaultMaterial(*i_mesh.material_data);
    }

    return mesh;
};
*/

}  // namespace Presto
