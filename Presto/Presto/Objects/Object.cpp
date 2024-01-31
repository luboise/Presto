#include "Object.h"

namespace Presto {
    uint32_t Object::_currentId = 0;

    Object::Object() {
        this->_id = Object::getNextAvailableId();
        PR_CORE_TRACE("Creating object with id {}", this->_id);
    }
    Object::~Object() {}

    uint32_t Object::getId() const { return this->_id; }

    uint32_t Object::getNextAvailableId() { return _currentId++; }
}  // namespace Presto
