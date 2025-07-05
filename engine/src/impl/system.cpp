module presto.system;
import presto.internal.managers.rendering_manager;

Pr::VisualExtents Pr::WindowSize() {
    using namespace Pr;

    // TODO: Fix this to come from the window instead of the rendering manager
    return RenderingManager::get().framebufferSize();
}

Pr::VisualExtents Pr::FramebufferSize() {
    using namespace Pr;

    return RenderingManager::get().framebufferSize();
}
