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
    return EntityManagerImpl::get().newEntity(name);
};

// MapFilterView<EntityManagerImpl::ComponentMap>
ComponentSearchResults EntityManager::findComponentsWhere(
    const ComponentFilter& filter) {
    return componentDatabase_ | std::views::values | std::views::join |
           std::views::filter(filter);
}

TransformData& TransformData::addRotation(Pr::vec3 r) {
    this->rotation = Quaternion::fromEuler(r) * this->rotation;
    return *this;
};

}  // namespace Pr
