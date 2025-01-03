#include "PrestoCore/Objects/EntityManager.h"
#include "PrestoCore/Events/ObjectEvents.h"

namespace Presto {
    // Static member declarations
    std::vector<entity_t> EntityManager::_entities;
    entity_id_t EntityManager::_currentId = 0;
    std::map<entity_id_t, entity_t> EntityManager::_entityMap;

    // Methods
    entity_t EntityManager::newEntity() {
        entity_id_t new_id = EntityManager::reserveId();

        PR_CORE_ASSERT(!_entityMap.contains(new_id),
                       "Attempted to create entity using existing id: {}",
                       new_id);

        auto new_entity = new Entity(new_id);
        _entities.push_back(new_entity);

        _entityMap.emplace(new_id, new_entity);

        return new_entity;
    }

    void EntityManager::destroyEntity(entity_t entity) {
        // Remove from map
        _entityMap.erase(entity->_id);

        // Remove from vector
        auto it = _entities.begin();
        while (it != _entities.end()) {
            if (*it == entity) {
                _entities.erase(it);
                break;
            }
            it++;
        }

        // Delete the entity
        delete entity;

        // Send event
        ObjectDestroyedEvent(static_cast<void*>(entity));
    }
    entity_id_t EntityManager::reserveId() { return _currentId++; }
}  // namespace Presto
