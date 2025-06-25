export module presto.internal.windowdata;

// For types/concepts
import presto.core;
import presto.core.event;

import presto.runtime.window;
import presto.internal.managers;

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
