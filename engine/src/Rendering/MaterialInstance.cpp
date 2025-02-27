#include <utility>

#include "Presto/Rendering/MaterialInstance.h"

namespace Presto {

MaterialInstance::MaterialInstance(const Ptr<MaterialAsset>& definition)
    : definition_(definition) {
    static_assert(
        false,
        "Material needs to be initialised with structure and uniform buffers.");
}

MaterialInstance& MaterialInstance::setProperty(Presto::string name,
                                                Presto::vec3 data) {
    return *this;
};

MaterialInstance& MaterialInstance::setProperty(Presto::string name,
                                                ErasedBytes data) {
    return *this;
};

MaterialInstance& MaterialInstance::setProperty(Presto::string name,
                                                Ptr<Texture> data) {
    return *this;
};

MaterialInstance& MaterialInstance::setName(Presto::string newName) {
    this->name_ = std::move(newName);
    return *this;
};

const MaterialStructure& MaterialInstance::getStructure() const {
    return definition_->getStructure();
};
}  // namespace Presto
