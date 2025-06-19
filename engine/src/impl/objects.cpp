export module presto.objects;

import presto.core.types;

namespace Presto {

Presto::CameraComponent& Presto::GetDefaultCamera() {
    using namespace Presto;

    return *RenderingManager::get()
                .getMainCamera()
                ->getComponent<CameraComponent>();
}

Presto::EntityRef Presto::NewLooseEntity() {
    return EntityManagerImpl::Get().newEntity();
};

Presto::EntityOwner Presto::NewOwnedEntity(Presto::vec3 pos) {
    EntityOwner new_owner{};
    new_owner->getComponent<TransformComponent>()->setTranslation(pos);

    return new_owner;
};

Presto::EntityPtr Presto::NewEntity(Presto::vec3 pos) {
    auto new_entity{EntityManagerImpl::Get().newEntity()};

    new_entity->getComponent<TransformComponent>()->setTranslation(pos);
    return new_entity;
};

void Presto::SetDefaultCameraConductor(
    const ComponentPtr<ConductorComponent>& ptr) {
    EntityPtr main_camera{RenderingManager::get().getMainCamera()};
    main_camera->setComponent<ConductorComponent>(ptr);
};

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

TransformData& TransformData::addRotation(Presto::vec3 r) {
    this->rotation = Quaternion::fromEuler(r) * this->rotation;
    return *this;
};

TransformData& TransformData::addRotation(Presto::Quaternion quat) {
    this->rotation = quat * this->rotation;
    return *this;
};

TransformData& TransformData::addTranslation(Presto::vec3 t) {
    this->position += t;
    return *this;
};
TransformData& TransformData::scaleBy(Presto::vec3 s) {
    this->scale *= s;
    return *this;
};

Presto::vec3 TransformData::forwards() const {
    return glm::normalize(rotation * vec3{0, 0, -1});
}

Presto::vec3 TransformData::backwards() const { return -forwards(); }

Presto::vec3 TransformData::leftwards() const {
    return glm::normalize(rotation * vec3{-1, 0, 0});
}

Presto::vec3 TransformData::rightwards() const { return -leftwards(); }

Presto::vec3 TransformData::upwards() const {
    return glm::normalize(rotation * vec3{0, 1, 0});
}

Presto::vec3 TransformData::downwards() const { return -upwards(); }

TransformData::TransformData() : TransformData({0, 0, 0}, Quaternion{}) {};

TransformData::TransformData(Presto::vec3 position, Quaternion rotation)
    : position{position}, rotation{rotation} {};

TransformData::TransformData(Presto::vec3 position, Presto::vec3 rotation)
    : position{position}, rotation{Quaternion::fromEuler(rotation)} {};

}  // namespace Presto
