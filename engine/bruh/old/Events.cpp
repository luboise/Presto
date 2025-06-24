#include "Presto/System.h"
#include "Runtime/WindowData.h"

#include "Presto/Events.h"

namespace Pr {

FramebufferResizedEvent::FramebufferResizedEvent(const WindowData* data)
    : windowData_(data),
      width_(windowData_->framebuffer_size.width),
      height_(windowData_->framebuffer_size.height) {}

MousePosition MouseMovedEvent::posNormalised() const {
    VisualExtents extents{Pr::FramebufferSize()};

    MousePosition ret{position_};
    ret.x /= static_cast<float>(extents.width);
    ret.y /= static_cast<float>(extents.height);

    return ret;
}

}  // namespace Pr
