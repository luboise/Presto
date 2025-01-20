#include "Presto/Components/Renderable/Mesh.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    void Mesh::setResource(MeshResource& resource) {
        if (!resource.isRegistered()) {
            RenderingManager::Get().loadMeshOnGpu(resource);
        }

        resource_ = &resource;
    }
    bool Mesh::hasResource() const { return resource_ != nullptr; }

    Mesh::Mesh(MeshResource& resource) { this->setResource(resource); };
}  // namespace Presto
