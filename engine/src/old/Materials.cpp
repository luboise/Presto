#include <utility>

#include "Modules/RenderingManager.h"
#include "Presto/Materials.h"

Presto::MaterialPtr Presto::NewMaterial(MaterialType type,
                                        Presto::string name) {
    using namespace Presto;

    return RenderingManager::get().createMaterial(type, std::move(name));
};

Presto::MaterialPtr Presto::FindMaterial(const Presto::string& name) {
    using namespace Presto;

    return RenderingManager::get().findMaterial(name);
};
