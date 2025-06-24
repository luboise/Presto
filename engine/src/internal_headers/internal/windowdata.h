// For types/concepts
#include "presto/core.h"
#include "presto/core/event.h"

#include "internal/managers.h"
#include "presto/runtime/window.h"

epoxrt namespace Pr {
    class Renderer;

    struct WindowData {
        Pr::string title;

        VisualExtents window_size;
        VisualExtents framebuffer_size;

        bool VSync{false};

        Renderer* pRenderer{nullptr};

        template <typename T>
            requires DerivedFrom<T, Event>
        void templated_event_callback(T event) {
            EventManagerImpl::get().handle<T>(event);
        };
    };

}  // namespace Pr
