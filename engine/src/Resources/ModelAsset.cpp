#include <utility>

#include "Presto/Resources/MeshResource.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    void ModelAsset::load() { RenderingManager::get().loadMeshOnGpu(*this); };

    ModelAsset::ModelAsset(resource_name_t name)
        : Asset(std::move(name)) {};
}  // namespace Presto
