#include "Presto/Assets/MeshAsset.h"

#include "Modules/ModelLoader.h"
#include "Presto/Core/Types.h"
#include "Presto/Modules/RenderingManager.h"

namespace Presto {

struct MeshAsset::Impl {
    BoundingBox box;

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

MeshAsset& MeshAsset::setVertices(const AttributeList& attributes) {
    if (!modifiable()) {
        return *this;
    }

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
