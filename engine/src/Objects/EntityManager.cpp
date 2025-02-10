#include "Presto/Modules/EntityManager.h"
#include <queue>

#include "Presto/Components/TransformComponent.h"
#include "Presto/Core/Assert.h"
#include "Presto/Objects/Component.h"
#include "Presto/Objects/Entity.h"
#include "Presto/Objects/Figure.h"
#include "Presto/Runtime/Events/ObjectEvents.h"

namespace Presto {
// Static member declarations
// std::vector<entity_ptr> EntityManager::entities_;
// entity_id_t EntityManager::_currentId = 0;
// std::map<entity_id_t, entity_ptr> EntityManager::entityMap_;

struct EntityManager::Impl {
    std::map<entity_id_t, entity_unique_ptr> entity_map;

    std::vector<entity_tag_name_t> tag_map;

    entity_id_t current_id{1};

    std::queue<entity_unique_ptr> entity_queue;
};

EntityManager::EntityManager() : impl_(new Impl()) {};

EntityManager::~EntityManager() {
    this->componentDatabase_.clear();
    this->impl_->entity_map.clear();
    this->impl_->tag_map.clear();
    delete impl_;
};

void EntityManager::update() {
    // TODO: Move this somewhere cached instead
    for (const auto& entity : impl_->entity_map | std::views::values) {
        for (const auto& script : entity->getConductors()) {
            script->update();
        }
    }
}

// Methods
entity_ptr EntityManager::newEntity(const entity_name_t& name) {
    entity_id_t new_id = EntityManager::reserveId();

    PR_CORE_ASSERT(
        std::ranges::none_of(impl_->entity_map | std::views::keys,
                             [new_id](auto& key) { return key == new_id; }),
        "Attempted to create entity using existing id: {}", new_id);

    auto new_entity = entity_unique_ptr(
        new Entity(new_id, name),
        [this](Entity* entity) { this->destroyEntity(entity); });

    const auto new_transform{newComponent<TransformComponent>()};
    new_entity->setComponent(new_transform);

    entity_ptr handle{new_entity.get()};
    PR_CORE_ASSERT(
        handle != nullptr,
        "Internal error: A new entity handle has been retrieved as null.");

    impl_->entity_queue.push(std::move(new_entity));

    return handle;
}

void EntityManager::destroyEntity(entity_ptr entity) {
    // // Remove from map
    // impl_->entity_map.erase(entity->id_);

    // Delete the entity
    delete entity;

    // Send event
    Presto::ObjectDestroyedEvent(static_cast<void*>(entity));
}

entity_id_t EntityManager::reserveId() { return impl_->current_id++; }

// TODO: Implement this to clean up dangling entities/components
void EntityManager::collectGarbage() {};

std::vector<entity_ptr> EntityManager::findAll() {
    std::vector<entity_ptr> entities(impl_->entity_map.size());
    int i = 0;

    // Get each raw pointer
    std::ranges::for_each(
        impl_->entity_map | std::views::values,
        [&entities, &i](auto& e) { entities[i++] = e.get(); });

    return entities;
};

std::vector<entity_ptr> EntityManager::findWhere(auto filter) {
    return impl_->entity_map | std::views::values | std::views::filter(filter);
}

// MapFilterView<EntityManager::ComponentMap>
ComponentSearchResults EntityManager::findComponentsWhere(
    const ComponentFilter& filter) {
    return componentDatabase_ | std::views::values | std::views::join |
           std::views::filter(filter);
}

/*
    MapFilterView<EntityManager::ComponentMap>
    EntityManager::findComponentsByType(CheckedComponentBits bits) {
        return components_ | std::views::values |
               std::views::filter([](auto& component) {
                   return component->hasBits(bits);
               })

};
                           */

entity_tag_id_t EntityManager::getTagId(
    const entity_tag_name_t& tagName) const {
    for (entity_tag_id_t i = 0;
         i < static_cast<entity_tag_id_t>(impl_->tag_map.size()); i++) {
        if (impl_->tag_map[i] == tagName) {
            return i;
        }
    }

    return INVALID_TAG_ID;
};

entity_tag_id_t EntityManager::createTag(const entity_tag_name_t& tagName) {
    PR_ASSERT(impl_->tag_map.size() < MAX_TAG_COUNT,
              std::format("Creating a new tag exceeds the maximum number "
                          "of tags allowed ({}).",
                          MAX_TAG_COUNT));

    PR_ASSERT(getTagId(tagName) != INVALID_TAG_ID,
              "Multiple tags can't be created with the same name.");

    const auto new_tag_index = impl_->tag_map.size();
    impl_->tag_map.push_back(tagName);

    return static_cast<entity_tag_id_t>(new_tag_index);
};

bool EntityManager::exists(entity_id_t id) const {
    return std::ranges::none_of(impl_->entity_map | std::views::keys,
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
    while (!impl_->entity_queue.empty()) {
        entity = std::move(impl_->entity_queue.front());
        impl_->entity_queue.pop();

        for (auto& components{entity->components_};
             auto& [key, component] : components) {
            PR_CORE_ASSERT(
                component != nullptr,
                "Null component found when instantiating new entities.");
            component->onEnterScene();
        }

        impl_->entity_map.emplace(entity->id_, std::move(entity));
    };
}
}  // namespace Presto
