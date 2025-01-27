#include "Presto/Components/Renderable/MeshGroup.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    /*
        void MeshGroup::setMesh(MeshResource& resource) {
            if (!resource.loaded()) {
                RenderingManager::get().loadMeshOnGpu(resource);
            }

            meshResource_ = &resource;
        }
            */

    // bool MeshGroup::hasResource() const { return meshResource_ != nullptr; }

    // const MeshResource& getMesh() { return *meshResource_; }

    MeshGroup::MeshGroup(MeshResource& resource) {
        this->setMesh(0, &resource);
    };
}  // namespace Presto
