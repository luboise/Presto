#include "Presto/Resources/MeshResource.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    void MeshResource::load() { RenderingManager::get().loadMeshOnGpu(*this); };
}  // namespace Presto
