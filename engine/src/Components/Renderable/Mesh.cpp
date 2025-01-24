#include "Presto/Components/Renderable/Mesh.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    void Mesh::setMesh(MeshResource& resource) {
        if (!resource.loaded()) {
            RenderingManager::get().loadMeshOnGpu(resource);
        }

        meshResource_ = &resource;
    }
    bool Mesh::hasResource() const { return meshResource_ != nullptr; }

    Mesh::Mesh(MeshResource& resource) { this->setMesh(resource); };
}  // namespace Presto
