export module presto.internal.window_data;

// For types/concepts
import presto.core;
import presto.core.concepts;
import presto.core.event;

import presto.types;

import presto.internal.managers.event_manager_impl;

export namespace Pr {
struct WindowData {
    Pr::string title;

    VisualExtents window_size;
    VisualExtents framebuffer_size;

    bool VSync{false};

    template <typename T>
        requires DerivedFrom<T, Event>
    void templated_event_callback(T event) {
        EventManagerImpl::get().handle<T>(event);
    };
};

}  // namespace Pr
