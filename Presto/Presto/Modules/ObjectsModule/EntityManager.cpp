#include "EntityManager.h"

namespace Presto {
    entity_t EntityManager::newEntity() { return new Entity(); }
}  // namespace Presto
