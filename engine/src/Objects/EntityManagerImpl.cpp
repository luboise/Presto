#include <queue>

#include "Modules/EntityManagerImpl.h"

#include "Presto/Core/Assert.h"
#include "Presto/Objects/Component.h"
#include "Presto/Objects/Components/TransformComponent.h"
#include "Presto/Objects/Entity.h"
#include "Presto/Objects/Figure.h"
#include "Presto/Runtime/Events/ObjectEvents.h"

namespace Presto {
// Static member declarations
// std::vector<entity_ptr> EntityManagerImpl::entities_;
// entity_id_t EntityManagerImpl::_currentId = 0;
// std::map<entity_id_t, entity_ptr> EntityManagerImpl::entityMap_;

struct EntityManagerImpl::Impl {
    std::map<entity_id_t, EntityPtr> entity_map;

    std::vector<entity_tag_name_t> tag_map;

    entity_id_t current_id{1};

    std::queue<EntityPtr> entity_queue;
};

EntityManager& EntityManager::Get() { return EntityManagerImpl::get(); }

EntityManagerImpl::EntityManagerImpl() : impl_(new Impl()) {};

EntityManagerImpl::~EntityManagerImpl() {
    this->componentDatabase_.clear();
    this->impl_->entity_map.clear();
    this->impl_->tag_map.clear();
    delete impl_;
};

void EntityManagerImpl::update() {
    // TODO: Move this somewhere cached instead
    for (const auto& entity : impl_->entity_map | std::views::values) {
        for (const auto& script : entity->getConductors()) {
            script->update();
        }
    }
}

// Methods
EntityPtr EntityManagerImpl::newEntity(const entity_name_t& name) {
    entity_id_t new_id = EntityManagerImpl::reserveId();

    PR_CORE_ASSERT(
        std::ranges::none_of(impl_->entity_map | std::views::keys,
                             [new_id](auto& key) { return key == new_id; }),
        "Attempted to create entity using existing id: {}", new_id);

    EntityPtr new_entity(new Entity(new_id, name));

    auto new_transform{newComponent<TransformComponent>()};
    new_entity->setComponent(new_transform);

    PR_CORE_ASSERT(
        new_entity.get() != nullptr,
        "Internal error: A new entity handle has been retrieved as nullptr.");

    impl_->entity_queue.push(new_entity);

    return new_entity;
}

void EntityManagerImpl::destroyEntity(Entity* entity_ptr) {
    // Delete the entity
    auto num_erased{impl_->entity_map.erase(entity_ptr->id_)};

    PR_CORE_ASSERT(num_erased == 1, std::format("Entity was not erased: {}",
                                                fmt::ptr(entity_ptr)));

    // Send event
    Presto::ObjectDestroyedEvent(static_cast<void*>(entity_ptr));
}

entity_id_t EntityManagerImpl::reserveId() { return impl_->current_id++; }

// TODO: Implement this to clean up dangling entities/components
void EntityManagerImpl::collectGarbage() {};

std::vector<EntityPtr> EntityManagerImpl::findAll() {
    auto vals{impl_->entity_map | std::views::values};
    return std::vector<EntityPtr>{vals.begin(), vals.end()};
};

std::vector<EntityPtr> EntityManagerImpl::findWhere(auto filter) {
    return impl_->entity_map | std::views::values | std::views::filter(filter);
}

/*
    MapFilterView<EntityManagerImpl::ComponentMap>
    EntityManagerImpl::findComponentsByType(CheckedComponentBits bits) {
        return components_ | std::views::values |
               std::views::filter([](auto& component) {
                   return component->hasBits(bits);
               })

};
                           */

entity_tag_id_t EntityManagerImpl::getTagId(
    const entity_tag_name_t& tagName) const {
    for (entity_tag_id_t i = 0;
         i < static_cast<entity_tag_id_t>(impl_->tag_map.size()); i++) {
        if (impl_->tag_map[i] == tagName) {
            return i;
        }
    }

    return INVALID_TAG_ID;
};

entity_tag_id_t EntityManagerImpl::createTag(const entity_tag_name_t& tagName) {
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

bool EntityManagerImpl::exists(entity_id_t id) const {
    return std::ranges::none_of(impl_->entity_map | std::views::keys,
                                [id](auto& key) { return key == id; });
};

std::vector<EntityPtr> EntityManagerImpl::newEntities(PR_SIZE count) {
    PR_CORE_ASSERT(count > 0 && count < PRESTO_FIGURE_MAX_ENTITY_COUNT,
                   "Invalid entity count construction requested.");
    std::vector<EntityPtr> entities(count);

    for (PR_SIZE i = 0; i < count; i++) {
        entities[i] = newEntity("Entity");
    }

    return entities;
};

void EntityManagerImpl::instantiateEntities() {
    EntityPtr entity{};
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

entity_id_t EntityManager::reserveId() {
    return EntityManagerImpl::get().reserveId();
};

EntityPtr EntityManager::newEntity(const entity_name_t& name) {
    return EntityManagerImpl::get().newEntity(name);
};

// MapFilterView<EntityManagerImpl::ComponentMap>
ComponentSearchResults EntityManager::findComponentsWhere(
    const ComponentFilter& filter) {
    return componentDatabase_ | std::views::values | std::views::join |
           std::views::filter(filter);
}

}  // namespace Presto
