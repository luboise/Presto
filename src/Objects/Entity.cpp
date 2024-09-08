#include "Presto/Objects/Entity.h"

#include "Presto/Events/ObjectEvents.h"

namespace Presto {
    Entity::Entity(entity_id_t id) {
        this->_id = id;
        ObjectCreatedEvent(this);
    }

    Entity::~Entity() {}

    uint32_t Entity::getId() const { return this->_id; }
}  // namespace Presto
