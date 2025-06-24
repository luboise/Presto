#include "Presto/Objects/Scene.h"

namespace Pr {
std::vector<EntityPtr> Scene::getEntities() const { return entityList_; };

}  // namespace Pr
