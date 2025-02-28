#include "Presto/Assets/MeshAsset.h"

#include "Modules/RenderingManager.h"
#include "Presto/Core/Constants.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Types/CoreTypes.h"

#include "Rendering/VertexProcessing.h"

namespace Presto {

struct MeshAsset::Impl {
    MeshDrawMode draw_mode{MeshDrawMode::TRIANGLES};
    ProcessedVertexData vertex_data;

    IndexList indices;

    BoundingBox box;
};

MeshAsset::MeshAsset() { impl_ = std::make_unique<Impl>(); };
MeshAsset::~MeshAsset() = default;

bool MeshAsset::load() {
    registrationId_ = RenderingManager::get().loadMesh(
        MeshData{.draw_mode = impl_->draw_mode,
                 .vertex_data{impl_->vertex_data},
                 .indices{impl_->indices}});

    return registrationId_ != PR_UNREGISTERED;
}

MeshAsset& MeshAsset::setDefaultMaterial(const MaterialPtr& material) {
    if (!modifiable()) {
        return *this;
    }

    defaultMaterial_ = material;
    return *this;
};

MeshAsset& MeshAsset::setDrawMode(MeshDrawMode mode) {
    impl_->draw_mode = mode;
    return *this;
};

// TODO: Implement bounding box logic on import of mesh
BoundingBox MeshAsset::getBoundingBox() const { return impl_->box; }

MeshAsset& MeshAsset::setVertices(const ImportedAttributeList& attributes) {
    if (!modifiable()) {
        return *this;
    }

    const PipelineStructure* ps{
        RenderingManager::get().getPipelineStructure(PR_PIPELINE_DEFAULT_3D)};

    auto processed{processVertices(attributes, *ps)};

    // TODO: Put checks here to make sure the processed vertices are well formed
    this->impl_->vertex_data = std::move(processed);

    // TODO: Implement actual bounding box
    impl_->box = {.x_min = -1,
                  .x_max = 1,
                  .y_min = -1,
                  .y_max = 1,
                  .z_min = -1,
                  .z_max = 1};

    return *this;
};

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

MeshAsset& MeshAsset::setIndices(IndexList indices) {
    this->impl_->indices = std::move(indices);
};

}  // namespace Presto
