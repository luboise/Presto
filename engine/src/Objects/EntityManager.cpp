#include "Presto/Modules/EntityManager.h"

#include "Presto/Components/Transform.h"
#include "Presto/Core/Assert.h"
#include "Presto/Objects/Entity.h"
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

    void EntityManager::update() {}

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

        Transform* new_transform = Transform::New();
        new_entity->setComponent(new_transform);

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

}  // namespace Presto
