#include "Presto/Rendering/MaterialInstance.h"

namespace Presto {

MaterialInstance::MaterialInstance(const Ptr<MaterialAsset>& definition)
    : definition_(definition) {
    static_assert(
        false,
        "Material needs to be initialised with structure and uniform buffers.");
}

}  // namespace Presto
