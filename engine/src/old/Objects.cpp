#include "Presto/Objects.h"
#include "Modules/RenderingManager.h"

#include "Modules/EntityManagerImpl.h"

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

void Presto::ConductorComponent::onEnterScene() {
    for (const auto& callback : preStartCallbacks_) {
        callback();
    }
    this->start();
}
void Presto::ConductorComponent::addPreStartCallback(
    const pre_start_callback_t& callback) {
    preStartCallbacks_.push_back(callback);
}
