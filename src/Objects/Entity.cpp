#include "Presto/Objects/Entity.h"

#include "Presto/Events/ObjectEvents.h"

namespace Presto {
    Entity::Entity() { ObjectCreatedEvent(this); }

    Entity::~Entity() = default;

    ComponentMap Entity::getComponents() { return _components; }

    uint32_t Entity::getId() const { return this->_id; }
}  // namespace Presto
