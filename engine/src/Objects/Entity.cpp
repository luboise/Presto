#include "Presto/Objects/Entity.h"

#include "Presto/Runtime.h"

using Presto::ObjectCreatedEvent;

namespace Presto {
    Entity::Entity(entity_id_t id) : id_(id) { ObjectCreatedEvent(this); }

    Entity::~Entity() = default;

    ComponentMap Entity::getComponents() { return components_; }
}  // namespace Presto
