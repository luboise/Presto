#pragma once

#include "Presto/Core/Concepts.h"
#include "Presto/Runtime/EventManager.h"

namespace Presto {

class EventListener {
    // Adds a new listener to the group
    template <typename E>
        requires DerivedFrom<E, Event>
    void addListener(EventHandler<E> handler) {
        EventManager::Get().addHandler(std::move(handler));
    };
};

}  // namespace Presto
