#include "presto/core/platform.h"

#include "event.h"
#include "event_macros.h"

#include "event_manager.h"

namespace Pr {

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

}  // namespace Pr
