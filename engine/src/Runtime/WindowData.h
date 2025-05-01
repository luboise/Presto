#pragma once

#include "Presto/Core/Concepts.h"
#include "Presto/Runtime/Events/Event.h"
#include "Presto/Runtime/Window.h"

#include "Modules/EventManagerImpl.h"

namespace Presto {

class Renderer;

struct WindowData {
    std::string title;

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
