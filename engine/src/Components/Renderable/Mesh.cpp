#include "Presto/Components/Renderable/Mesh.h"

#include "Presto/Modules/RenderingManager.h"
#include "Presto/Rendering/RenderData.h"

namespace Presto {
    RenderData Mesh::getRenderData() const {
        // TODO: Implement this
        return {};
    };

    void Mesh::setResource(MeshResource& resource) {
        if (!resource_->isRegistered()) {
            RenderingManager::Get().loadMeshOnGpu(resource);
        }
    }
    bool Mesh::hasResource() const { return resource_ == nullptr; }

    Mesh::Mesh(MeshResource& resource) { this->setResource(resource); };
}  // namespace Presto
