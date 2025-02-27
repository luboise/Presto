#include "Presto/Assets/MeshAsset.h"

#include "Modules/RenderingManager.h"
#include "Presto/Core/Constants.h"
#include "Presto/Types/CoreTypes.h"

#include "Rendering/VertexProcessing.h"

namespace Presto {

struct MeshAsset::Impl {
    BoundingBox box;

    ProcessedVertexData vertex_data;

    ByteArray vertices;
    Presto::size_t vertex_count{0};

    IndexList indices;
};

MeshAsset::MeshAsset() { impl_ = std::make_unique<Impl>(); };

bool MeshAsset::load() {
    RenderingManager::get().loadMeshOnGpu(*this);
    return true;
}

MeshAsset& MeshAsset::setDefaultMaterial(const MaterialPtr& material) {
    if (!modifiable()) {
        return *this;
    }

    defaultMaterial_ = material;
};

BoundingBox MeshAsset::getBoundingBox() const { return impl_->box; };

MeshAsset& MeshAsset::setVertices(const ImportedAttributeList& attributes) {
    if (!modifiable()) {
        return *this;
    }

    const PipelineStructure* ps{
        RenderingManager::get().getPipelineStructure(PR_PIPELINE_DEFAULT_3D)};

    auto processed{processVertices(attributes, ps->attributes)};

    // TODO: Put checks here to make sure the processed vertices are well formed
    this->impl_->vertex_data = std::move(processed);

    // TODO: Implement actual bounding box
    impl_->box = {.x_min = -1,
                  .x_max = 1,
                  .y_min = -1,
                  .y_max = 1,
                  .z_min = -1,
                  .z_max = 1};
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

}  // namespace Presto
