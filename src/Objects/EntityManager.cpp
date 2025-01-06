#include "Presto/Modules/EntityManager.h"
#include <algorithm>
#include "Presto/Runtime/Events/ObjectEvents.h"

namespace Presto {
    // Static member declarations
    // std::vector<entity_ptr> EntityManager::entities_;
    // entity_id_t EntityManager::_currentId = 0;
    // std::map<entity_id_t, entity_ptr> EntityManager::entityMap_;

    // Methods
    entity_ptr EntityManager::newEntity() {
        entity_id_t new_id = EntityManager::reserveId();

        PR_CORE_ASSERT(
            std::ranges::none_of(entityMap_ | std::views::keys,
                                 [new_id](auto& key) { return key == new_id; }),
            "Attempted to create entity using existing id: {}", new_id);

        auto new_entity = entity_unique_ptr(
            new Entity(new_id),
            [this](Entity* entity) { this->destroyEntity(entity); });

        entityMap_.emplace(new_id, std::move(new_entity));
        return entityMap_[new_id].get();
    }

    void EntityManager::destroyEntity(entity_ptr entity) {
        // Remove from map
        entityMap_.erase(entity->id_);

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
        Presto::ObjectDestroyedEvent(static_cast<void*>(entity));
    }

    void EntityManager::Init() {
        PR_CORE_INFO("Initialising EntityManager.");
        instance_ = std::unique_ptr<EntityManager>(new EntityManager());
    }

    void EntityManager::Update() {}

    entity_id_t EntityManager::reserveId() { return _currentId++; }

    // TODO: Implement this to clean up dangling entities/components
    void EntityManager::collectGarbage() {};
    std::vector<entity_ptr> EntityManager::findAll() {
        std::vector<entity_ptr> entities(entityMap_.size());
        int i = 0;

        // Get each raw pointer
        std::ranges::for_each(
            entityMap_ | std::views::values,
            [&entities, &i](auto& e) { entities[i++] = e.get(); });

        return entities;
    };

    std::vector<entity_ptr> EntityManager::findWhere(auto filter) {
        return entityMap_ | std::views::values | std::views::filter(filter);
    }

    std::vector<component_ptr> EntityManager::findComponentsWhere(auto filter) {
        return components_ | std::views::filter(filter);
    }

}  // namespace Presto
