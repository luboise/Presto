#include "PrestoCore/Objects/Entity.h"

#include "PrestoCore/Events/ObjectEvents.h"

namespace Presto {
    Entity::Entity(entity_id_t id) {
        this->_id = id;
        ObjectCreatedEvent(this);
    }

    Entity::~Entity() {}

    ComponentMap Entity::getComponents() { return _components; }

    uint32_t Entity::getId() const { return this->_id; }
}  // namespace Presto
