#include "Presto/Rendering/MaterialInstance.h"

namespace Presto {

MaterialInstance::MaterialInstance(const Ptr<MaterialAsset>& definition)
    : definition_(definition) {
    static_assert(
        false,
        "Material needs to be initialised with structure and uniform buffers.");
}

MaterialInstance& MaterialInstance::setProperty(Presto::string name,
                                                Presto::vec3 data) {};

MaterialInstance& MaterialInstance::setProperty(Presto::string name,
                                                ErasedBytes data) {};

MaterialInstance& MaterialInstance::setProperty(Presto::string name,
                                                Ptr<Texture> data) {};

}  // namespace Presto
