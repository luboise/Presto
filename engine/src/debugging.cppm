export module presto.internal.debugging;

export import :debug_constants;

import presto.types.core;
import presto.objects.components.camera;

import presto.internal.managers.rendering_manager;

import std;

export namespace Pr {

class DebugCameraListener {
   public:
    DebugCameraListener();
    explicit DebugCameraListener(RenderingManager&);
    void toggle();

    [[nodiscard]] bool enabled() const;

    CameraComponent& camera();

   private:
    bool enabled_{false};
    Ptr<CameraComponent> camera_;
};

}  // namespace Pr

module :private;

namespace Pr {

DebugCameraListener::DebugCameraListener() { camera_ = nullptr; };

DebugCameraListener::DebugCameraListener(RenderingManager& rm) {
    camera_ = rm.getDebugCamera();
};

void DebugCameraListener::toggle() {
    if (camera_ == nullptr) {
        Pr::CoreLog(ERROR, "Unable to doggle debug camera on, as it is null.");
        return;
    }

    enabled_ = !enabled_;
    RenderingManager::get().setUsingDebugCamera(enabled_);
};

bool DebugCameraListener::enabled() const { return enabled_; };

CameraComponent& DebugCameraListener::camera() { return *camera_; };
}  // namespace Pr
