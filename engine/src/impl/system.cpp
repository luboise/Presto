module presto.system;
import presto.internal.managers.rendering;

Presto::VisualExtents Presto::WindowSize() {
    using namespace Presto;

    // TODO: Fix this to come from the window instead of the rendering manager
    return RenderingManager::get().framebufferSize();
}

Presto::VisualExtents Presto::FramebufferSize() {
    using namespace Presto;

    return RenderingManager::get().framebufferSize();
}
