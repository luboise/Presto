#include "Entity.h"

namespace Presto {
    uint32_t Entity::_currentId = 0;

    Entity::Entity() {
        this->_id = Entity::getNextAvailableId();
        PR_CORE_TRACE("Creating object with id {}", this->_id);
    }
    Entity::~Entity() {}

    uint32_t Entity::getId() const { return this->_id; }

    uint32_t Entity::getNextAvailableId() { return _currentId++; }
}  // namespace Presto
