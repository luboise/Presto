#include "Presto/Objects/Scene.h"

namespace Presto {
    std::vector<entity_ptr> Scene::getEntities() const { return entityList_; };

}  // namespace Presto
