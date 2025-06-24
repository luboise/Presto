// For types/concepts
#include "presto/core.h"
#include "presto/core/event.h"

#include "presto/internal/managers.h"
#include "presto/runtime/window.h"

epoxrt namespace Presto {
    class Renderer;

    struct WindowData {
        Presto::string title;

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

}  // namespace Presto
