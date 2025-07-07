module presto.internal.managers.entity_manager_impl;

import presto.objects;
import presto.objects.components;

import presto.types;

import presto.internal;

import presto.core;

namespace Pr {
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

EntityManagerImpl::EntityManagerImpl() : impl_(new Impl()) {};

EntityManagerImpl::~EntityManagerImpl() {
    this->componentDatabase_.clear();
    this->impl_->entity_map.clear();
    this->impl_->tag_map.clear();
    delete impl_;
};

// Methods
void EntityManagerImpl::destroyEntity(Entity* entity_ptr) {
    // Delete the entity
    auto num_erased{impl_->entity_map.erase(entity_ptr->id())};

    Pr::CoreAssert(num_erased == 1, std::format("Entity was not erased: 0x{}",
                                                (Pr::uint64_t)(entity_ptr)));

    // Send event
    Pr::ObjectDestroyedEvent(static_cast<void*>(entity_ptr));
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
    Pr::Assert(impl_->tag_map.size() < MAX_TAG_COUNT,
               std::format("Creating a new tag exceeds the maximum number "
                           "of tags allowed ({}).",
                           MAX_TAG_COUNT));

    Pr::Assert(getTagId(tagName) != INVALID_TAG_ID,
               "Multiple tags can't be created with the same name.");

    const auto new_tag_index = impl_->tag_map.size();
    impl_->tag_map.push_back(tagName);

    return static_cast<entity_tag_id_t>(new_tag_index);
};

bool EntityManagerImpl::exists(entity_id_t id) const {
    return std::ranges::none_of(impl_->entity_map | std::views::keys,
                                [id](auto& key) { return key == id; });
};

}  // namespace Pr
