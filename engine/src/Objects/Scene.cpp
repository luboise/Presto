#include "Presto/Objects/Scene.h"

namespace Presto {
std::vector<EntityPtr> Scene::getEntities() const { return entityList_; };

}  // namespace Presto
