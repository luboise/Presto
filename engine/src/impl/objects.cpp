module presto.objects;

import std;

import presto.math;
import presto.types.core;

import presto.internal.managers;

import presto.objects.components.conductor;

std::vector<Ptr<ConductorComponent>> Pr::GetConductors() {
    auto data{getComponents() | std::views::values |
              std::views::transform(
                  [](auto& val) -> ComponentPtr<ConductorComponent> {
                      return std::dynamic_pointer_cast<ConductorComponent>(val);
                  }) |
              std::views::filter(
                  [](const auto& val) -> bool { return val != nullptr; })};

    return {data.begin(), data.end()};
}

Pr::CameraComponent& Pr::GetDefaultCamera() {
    using namespace Pr;

    return *RenderingManager::get()
                .getMainCamera()
                ->getComponent<CameraComponent>();
}

Pr::EntityRef Pr::NewLooseEntity() {
    return EntityManagerImpl::Get().newEntity();
};

Pr::EntityOwner Pr::NewOwnedEntity(Pr::vec3 pos) {
    EntityOwner new_owner{};
    new_owner->getComponent<TransformComponent>()->setTranslation(pos);

    return new_owner;
};

Pr::EntityPtr Pr::NewEntity(Pr::vec3 pos) {
    auto new_entity{EntityManagerImpl::Get().newEntity()};

    new_entity->getComponent<TransformComponent>()->setTranslation(pos);
    return new_entity;
};

void Pr::SetDefaultCameraConductor(
    const ComponentPtr<ConductorComponent>& ptr) {
    EntityPtr main_camera{RenderingManager::get().getMainCamera()};
    main_camera->setComponent<ConductorComponent>(ptr);
};

namespace Pr {

EntityManager& EntityManager::Get() { return EntityManagerImpl::get(); }

vec3 applyTransformation(const vec3& v, const mat4& transformations) {
    vec4 transformed = transformations * vec4{v, 1};
    transformed /= transformed.w;

    return vec3{transformed};
};

vec3 applyRotations(const vec3& v, const vec3& rotations) {
    mat4 transformation{1};

    return rotations * v;

    transformation =
        glm::rotate(transformation, glm::radians(rotations.z), {0, 0, 1});
    transformation =
        glm::rotate(transformation, glm::radians(rotations.y), {0, 1, 0});
    transformation =
        glm::rotate(transformation, glm::radians(rotations.x), {1, 0, 0});

    return applyTransformation(v, transformation);
}

entity_id_t EntityManager::reserveId() {
    return EntityManagerImpl::get().reserveId();
};

EntityPtr EntityManager::newEntity(const entity_name_t& name) {
    entity_id_t new_id = EntityManagerImpl::reserveId();

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

void EntityManager::newEntities(Pr::size_t count) {
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

}  // namespace Pr
