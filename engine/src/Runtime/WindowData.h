#pragma once

#include "Modules/EventManager.h"

#include "Presto/Core/Concepts.h"
#include "Window.h"

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
        EventManager::get().handle<T>(event);
    };
};

}  // namespace Presto
