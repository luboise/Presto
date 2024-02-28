#include "Entity.h"

#include "Presto/Events/ObjectEvents.h"

namespace Presto {
    uint32_t Entity::_currentId = 0;

    Entity::Entity() {
        this->_id = Entity::getNextAvailableId();

        ObjectCreatedEvent(this);
    }
    Entity::~Entity() {}

    uint32_t Entity::getId() const { return this->_id; }

    uint32_t Entity::getNextAvailableId() { return _currentId++; }
}  // namespace Presto
