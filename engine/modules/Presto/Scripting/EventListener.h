#pragma once

#include "Presto/Core/Concepts.h"
#include "Presto/Runtime/EventManager.h"

namespace Presto {

class EventListener {
   public:
    // Adds a new listener to the group
    template <typename E>
        requires DerivedFrom<E, Event>
    void on(EventHandler<E> handler) {
        EventManager::Get().addHandler(std::move(handler));
    };
};

}  // namespace Presto
