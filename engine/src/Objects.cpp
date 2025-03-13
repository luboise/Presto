#include "Presto/Objects.h"
#include "Modules/RenderingManager.h"

#include "Modules/EntityManagerImpl.h"

Presto::CameraComponent& Presto::GetDefaultCamera() {
    using namespace Presto;

    return *RenderingManager::get()
                .getMainCamera()
                ->getComponent<CameraComponent>();
}

Presto::EntityRef Presto::NewEntity() {
    return EntityManagerImpl::Get().newEntity();
};

void Presto::SetDefaultCameraConductor(
    const ComponentPtr<ConductorComponent>& ptr) {
    EntityPtr main_camera{RenderingManager::get().getMainCamera()};
    main_camera->setComponent<ConductorComponent>(ptr);
};
