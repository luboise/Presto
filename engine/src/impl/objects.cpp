export module presto.objects;

namespace Presto {

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

}  // namespace Presto
