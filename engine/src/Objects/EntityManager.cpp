#include "Presto/Modules/EntityManager.h"

#include "Presto/Components/ConductorComponent.h"
#include "Presto/Components/TransformComponent.h"
#include "Presto/Core/Assert.h"
#include "Presto/Objects/Entity.h"
#include "Presto/Objects/Figure.h"
#include "Presto/Runtime/Events/ObjectEvents.h"

namespace Presto {
    // Static member declarations
    // std::vector<entity_ptr> EntityManager::entities_;
    // entity_id_t EntityManager::_currentId = 0;
    // std::map<entity_id_t, entity_ptr> EntityManager::entityMap_;

    void EntityManager::init() {
        PR_CORE_INFO("Initialising EntityManager.");
        instance_ = std::unique_ptr<EntityManager>(new EntityManager());
    }

    void EntityManager::update() {
        for (auto&& [key, value] : entityMap_) {
            if (auto* script = value->getComponent<ConductorComponent>();
                script != nullptr) {
                script->update();
            }
        }

        /*
for (auto&& [key, value] : entityMap_) {
    if (auto* script = value->getComponent<ConductorComponent>();
        script != nullptr) {
        script->update();
    }
}
        */
    }

    void EntityManager::shutdown() {
        instance_->entities_.clear();
        instance_->components_.clear();
        instance_->entityMap_.clear();
        instance_->tagMap_.clear();
    }

    // Methods
    entity_ptr EntityManager::newEntity(const entity_name_t& name) {
        entity_id_t new_id = EntityManager::reserveId();

        PR_CORE_ASSERT(
            std::ranges::none_of(entityMap_ | std::views::keys,
                                 [new_id](auto& key) { return key == new_id; }),
            "Attempted to create entity using existing id: {}", new_id);

        auto new_entity = entity_unique_ptr(
            new Entity(new_id, name),
            [this](Entity* entity) { this->destroyEntity(entity); });

        auto* new_transform{newComponent<TransformComponent>()};
        new_entity->setComponent(new_transform);

        entity_ptr handle{new_entity.get()};
        PR_CORE_ASSERT(
            handle != nullptr,
            "Internal error: A new entity handle has been retrieved as null.");

        entityQueue_.push(std::move(new_entity));

        return handle;
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

    MapFilterView<EntityManager::ComponentMap>
    EntityManager::findComponentsWhere(const MapFilter<ComponentMap>& filter) {
        return components_ | std::views::values | std::views::filter(filter);
    }

    entity_tag_id_t EntityManager::getTagId(
        const entity_tag_name_t& tagName) const {
        for (entity_tag_id_t i = 0;
             i < static_cast<entity_tag_id_t>(tagMap_.size()); i++) {
            if (tagMap_[i] == tagName) {
                return i;
            }
        }

        return INVALID_TAG_ID;
    };

    entity_tag_id_t EntityManager::createTag(const entity_tag_name_t& tagName) {
        PR_ASSERT(tagMap_.size() < MAX_TAG_COUNT,
                  std::format("Creating a new tag exceeds the maximum number "
                              "of tags allowed ({}).",
                              MAX_TAG_COUNT));

        PR_ASSERT(getTagId(tagName) != INVALID_TAG_ID,
                  "Multiple tags can't be created with the same name.");

        auto new_tag_index = tagMap_.size();
        tagMap_.push_back(tagName);

        return static_cast<entity_tag_id_t>(new_tag_index);
    };

    bool EntityManager::exists(entity_id_t id) const {
        return std::ranges::none_of(entityMap_ | std::views::keys,
                                    [id](auto& key) { return key == id; });
    };

    std::vector<Entity*> EntityManager::newEntities(PR_SIZE count) {
        PR_CORE_ASSERT(count > 0 && count < PRESTO_FIGURE_MAX_ENTITY_COUNT,
                       "Invalid entity count construction requested.");
        std::vector<Entity*> entities(count);

        for (PR_SIZE i = 0; i < count; i++) {
            entities[i] = newEntity("Entity");
        }

        return entities;
    };

    void EntityManager::instantiateEntities() {
        entity_unique_ptr entity{};
        while (!entityQueue_.empty()) {
            entity = std::move(entityQueue_.front());
            entityQueue_.pop();

            auto& components{entity->components_};

            for (auto& [key, component] : components) {
                component->onEnterScene();
            }

            entityMap_.emplace(entity->id_, std::move(entity));
        };
    }
}  // namespace Presto
