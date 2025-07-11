module presto.objects.base;

import glm;

import presto.math;

import presto.objects;
import presto.objects.components;
import presto.objects.figure;

import presto.internal.events;
import presto.internal.managers.entity_manager_impl;
import presto.internal.managers.physics_manager;

namespace Pr {

struct EntityManager::Impl {
    std::map<entity_id_t, EntityPtr> entity_map;

    std::vector<entity_tag_name_t> tag_map;

    entity_id_t current_id{1};

    std::queue<EntityPtr> entity_queue;
};

EntityManager::EntityManager() : impl_(new Impl()) {};

EntityManager::~EntityManager() {
    this->componentDatabase_.clear();
    this->impl_->entity_map.clear();
    this->impl_->tag_map.clear();
    delete impl_;
};

EntityManager& EntityManager::Get() { return EntityManagerImpl::get(); }

vec3 applyTransformation(const vec3& v, const mat4& transformations) {
    vec4 transformed = transformations * vec4{v, 1};
    transformed /= transformed.w;

    return vec3{transformed};
};

vec3 applyRotations(const vec3& v, const vec3& rotations) {
    mat4 transformation{1};

    return rotations * v;

    transformation = glm::ext::rotate(
        transformation, Pr::Math::Radians(rotations.z), {0, 0, 1});
    transformation = glm::ext::rotate(
        transformation, Pr::Math::Radians(rotations.y), {0, 1, 0});
    transformation = glm::ext::rotate(
        transformation, Pr::Math::Radians(rotations.x), {1, 0, 0});

    return applyTransformation(v, transformation);
}

entity_id_t EntityManager::reserveId() { return impl_->current_id++; };

EntityPtr EntityManager::newEntity(const entity_name_t& name) {
    entity_id_t new_id{reserveId()};

    Pr::CoreAssert(
        std::ranges::none_of(impl_->entity_map | std::views::keys,
                             [new_id](auto& key) { return key == new_id; }),
        "Attempted to create entity using existing id: {}", new_id);

    EntityPtr new_entity(new Entity(new_id, name));

    auto new_transform{newComponent<TransformComponent>()};
    new_entity->setComponent(new_transform);

    Pr::CoreAssert(
        new_entity.get() != nullptr,
        "Internal error: A new entity handle has been retrieved as nullptr.");

    impl_->entity_queue.push(new_entity);

    return new_entity;
};

// MapFilterView<EntityManagerImpl::ComponentMap>
ComponentSearchResults EntityManager::findComponentsWhere(
    const ComponentFilter& filter) {
    return componentDatabase_ | std::views::values | std::views::join |
           std::views::filter(filter);
}

void EntityManager::instantiateEntities() {
    EntityPtr entity{};
    while (!impl_->entity_queue.empty()) {
        entity = std::move(impl_->entity_queue.front());
        impl_->entity_queue.pop();

        for (auto& components{entity->components()};
             auto& [key, component] : components) {
            Pr::CoreAssert(
                component != nullptr,
                "Null component found when instantiating new entities.");
            component->onEnterScene();
            component->enteredScene_ = true;
        }

        impl_->entity_map.emplace(entity->id(), std::move(entity));
    };
}

std::vector<EntityPtr> EntityManager::newEntities(Pr::size_t count) {
    Pr::CoreAssert(count > 0 && count < PRESTO_FIGURE_MAX_ENTITY_COUNT,
                   "Invalid entity count construction requested.");
    std::vector<EntityPtr> entities(count);

    for (Pr::size_t i = 0; i < count; i++) {
        entities[i] = newEntity("Entity");
    }

    return entities;
}

void EntityManager::update() {
    // TODO: Move this somewhere cached instead
    for (const auto& entity : impl_->entity_map | std::views::values) {
        for (const auto& script : Pr::GetConductors(entity)) {
            script->update();
        }
    }
}

Entity::Entity(entity_id_t id, entity_name_t name)
    : name_(std::move(name)), id_(id) {
    ObjectCreatedEvent(this);
}

Entity::~Entity() = default;

void Entity::addTag(const entity_tag_name_t& tagName) {
    const entity_tag_id_t tag_id{EntityManager::Get().getTagId(tagName)};
    Pr::Assert(tag_id != INVALID_TAG_ID,
               std::format("Unable to get tag id for name {}. Has the tag "
                           "been created yet?",
                           tagName));

    addTag(tag_id);
}

void Entity::addTag(entity_tag_id_t tag) { tags_[tag] = true; };

Entity::ComponentMap& Entity::components() { return components_; }

void Entity::checkNewComponent(GenericComponentPtr componentPtr) {
    // auto* conductor_ptr{dynamic_cast<ConductorComponent*>(componentPtr)};
    const auto conductor_ptr{
        std::dynamic_pointer_cast<ConductorComponent>(componentPtr)};

    // Handle new conductors
    if (conductor_ptr != nullptr) {
        for (const Ptr<ConductorComponent>& conductor :
             Pr::GetConductors(EntityPtr{this})) {
            if (conductor->registered_) {
                Pr::Assert(
                    this == conductor->entity,
                    "A Conductor component has been assigned to multiple "
                    "different entities.");
                continue;
            }

            conductor->registered_ = true;
            conductor->entity = this;
        }
    }

    auto rigidbody_ptr{
        std::dynamic_pointer_cast<RigidBodyComponent>(componentPtr)};

    if (rigidbody_ptr != nullptr) {
        PhysicsManager::get().addPairing(
            {.entity = this, .body = rigidbody_ptr});
    }
};

// Methods
void EntityManager::destroyEntity(Entity* entity_ptr) {
    // Delete the entity
    auto num_erased{impl_->entity_map.erase(entity_ptr->id())};

    Pr::CoreAssert(num_erased == 1, std::format("Entity was not erased: 0x{}",
                                                (Pr::uint64_t)(entity_ptr)));

    // Send event
    Pr::ObjectDestroyedEvent(static_cast<void*>(entity_ptr));
}

std::vector<EntityPtr> EntityManager::findAll() {
    auto vals{impl_->entity_map | std::views::values};
    return std::vector<EntityPtr>{vals.begin(), vals.end()};
};

void Entity::destroy() { EntityManager::Get().destroyEntity(this); }

void ConductorComponent::onEnterScene() {
    for (const auto& callback : preStartCallbacks_) {
        callback();
    }
    this->start();
}

void ConductorComponent::addPreStartCallback(
    const pre_start_callback_t& callback) {
    preStartCallbacks_.push_back(callback);
}

bool EntityManager::exists(entity_id_t id) const {
    return std::ranges::none_of(impl_->entity_map | std::views::keys,
                                [id](auto& key) { return key == id; });
};

std::vector<EntityPtr> EntityManager::findWhere(auto filter) {
    return impl_->entity_map | std::views::values | std::views::filter(filter);
}

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

}  // namespace Pr
