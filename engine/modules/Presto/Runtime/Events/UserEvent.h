#pragma once

#include "Event.h"
#include "Presto/Runtime/EventManager.h"

namespace Presto {

template <typename T>
class PRESTO_API UserEvent : public Event {
    // EventDispatcher can view private members, needed to dispatch the
    // events
    friend class EventDispatcher;

    EVENT_CLASS_CATEGORY(EventCategoryUser)
    EVENT_USER_CLASS_TYPE(#T)

   public:
    void dispatch() { EventManager::Get().handle<T>(*static_cast<T*>(this)); };
};

}  // namespace Presto
