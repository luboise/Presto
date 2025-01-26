#include <utility>

#include "Presto/Resources/MeshResource.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    void MeshResource::load() { RenderingManager::get().loadMeshOnGpu(*this); };

    MeshResource::MeshResource(resource_name_t name)
        : Resource(std::move(name)) {};
}  // namespace Presto
