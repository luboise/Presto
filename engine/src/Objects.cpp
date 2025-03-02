#include "Presto/Objects.h"
#include "Modules/RenderingManager.h"

Presto::CameraComponent& Presto::GetDefaultCamera() {
    using namespace Presto;

    return RenderingManager::get().getCamera();
}
