#include "Runtime/WindowData.h"

#include "Presto/Events.h"

namespace Presto {

FramebufferResizedEvent::FramebufferResizedEvent(const WindowData* data)
    : windowData_(data),
      width_(windowData_->framebuffer_size.width),
      height_(windowData_->framebuffer_size.height) {}

}  // namespace Presto
