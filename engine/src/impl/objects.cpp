module presto.objects;

import std;

import presto.core.assert;

import presto.math;
import presto.types.core;

import presto.internal.managers;

import presto.types.handles;

import presto.objects.base;
import presto.objects.components.transform;

using Pr::Ptr, Pr::ConductorComponent;

std::vector<Ptr<ConductorComponent>> Pr::GetConductors(EntityPtr entity) {
    auto data{entity->components() | std::views::values |
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

Entity* EntityOwner::operator->() { return entity_.get(); }

EntityOwner::EntityOwner() { entity_ = EntityManagerImpl::get().newEntity(); };

EntityOwner::~EntityOwner() {
    if (entity_ != nullptr) {
        entity_->destroy();
    }
};

EntityOwner& EntityOwner::operator=(EntityOwner&& other) noexcept {
    this->entity_ = other.entity_;
    other.entity_ = nullptr;

    return *this;
};

EntityOwner::EntityOwner(EntityOwner&& other) noexcept
    : entity_(std::move(other.entity_)) {
    other.entity_ = nullptr;
};

}  // namespace Pr
