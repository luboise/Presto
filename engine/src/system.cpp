#include "presto/system.h"
#include "internal/managers/rendering_manager.h"

Pr::VisualExtents Pr::WindowSize() {
    using namespace Pr;

    // TODO: Fix this to come from the window instead of the rendering manager
    return RenderingManager::get().framebufferSize();
}

Pr::VisualExtents Pr::FramebufferSize() {
    using namespace Pr;

    return RenderingManager::get().framebufferSize();
}
