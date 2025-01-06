#include "Presto/Modules/EntityManager.h"
#include "Presto/Runtime/Events/ObjectEvents.h"

namespace Presto {
    // Static member declarations
    std::vector<entity_ptr> EntityManager::entities_;
    entity_id_t EntityManager::_currentId = 0;
    std::map<entity_id_t, entity_ptr> EntityManager::entityMap_;

    // Methods
    entity_ptr EntityManager::newEntity() {
        entity_id_t new_id = EntityManager::reserveId();

        PR_CORE_ASSERT(!entityMap_.contains(new_id),
                       "Attempted to create entity using existing id: {}",
                       new_id);

        auto *new_entity{new Entity(new_id)};
        entities_.push_back(new_entity);

        entityMap_.emplace(new_id, new_entity);

        return new_entity;
    }

    void EntityManager::destroyEntity(entity_ptr entity) {
        // Remove from map
        entityMap_.erase(entity->_id);

        // Remove from vector
        auto it = entities_.begin();
        while (it != entities_.end()) {
            if (*it == entity) {
                entities_.erase(it);
                break;
            }
            it++;
        }

        // Delete the entity
        delete entity;

        // Send event
        Presto::ObjectDestroyedEvent(static_cast<void *>(entity));
    }

    void EntityManager::Init() {
        PR_CORE_INFO("Initialising EntityManager.");
        instance_ = std::unique_ptr<EntityManager>(new EntityManager());
    }

    void EntityManager::Update() {}

    entity_id_t EntityManager::reserveId() { return _currentId++; }
}  // namespace Presto
