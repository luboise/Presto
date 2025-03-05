#include "Presto/Objects.h"
#include "Modules/RenderingManager.h"

#include "Modules/EntityManagerImpl.h"

Presto::CameraComponent& Presto::GetDefaultCamera() {
    using namespace Presto;

    return RenderingManager::get().getCamera();
}

Presto::EntityPtr Presto::NewEntity() {
    return EntityManagerImpl::Get().newEntity();
};
