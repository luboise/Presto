module;
#include <functional>
#include <iostream>

export module presto.core.event.base;

import presto.core.types;
import presto.core.constants;
import presto.core.concepts;

export namespace Pr {

enum class EventType {
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLoseFocus,
    WindowMoved,
    FramebufferResized,

    AppTick,
    AppUpdate,
    AppRender,

    GamepadBtnPressed,
    GamepadBtnReleased,

    Key,

    MouseBtnPressed,
    MouseBtnReleased,
    MouseMoved,
    MouseScrolled,

    ObjectCreated,
    ObjectDestroyed,

    User,
};

using event_category_t = PR_BIT_TYPE;

enum EventCategory : event_category_t {
    None = AS_BIT(0),
    EventCategoryApplication = AS_BIT(1),

    EventCategoryInput = AS_BIT(2),
    EventCategoryGamepad = AS_BIT(3),
    EventCategoryKeyboard = AS_BIT(4),
    EventCategoryMouse = AS_BIT(5),
    EventCategoryMouseButton = AS_BIT(6),

    EventCategoryObject = AS_BIT(7),

    EventCategoryUser = AS_BIT(8)
};

class Event {
    // EventDispatcher can view private members, needed to dispatch the
    // events
    friend class EventDispatcher;

   public:
    [[nodiscard]] virtual EventType getType() const = 0;
    [[nodiscard]] virtual const char* getName() const = 0;
    [[nodiscard]] virtual event_category_t getCategories() const = 0;
    [[nodiscard]] virtual Pr::string toString() const { return getName(); };

    [[nodiscard]] bool inCategory(EventCategory category) const {
        return (getCategories() & category) != 0;
    }

    virtual ~Event() = default;

   protected:
    bool event_handled = false;
};

template <typename T>
concept EventInstanceType = std::is_base_of_v<Event, T>;

template <typename T>
    requires DerivedFrom<T, Event>
using EventHandler = std::function<void(T&)>;

inline std::ostream& operator<<(std::ostream& os, const Event& event) {
    return os << event.toString();
}

/*
class EventDispatcher {
    template <typename T>
    // Reserved function pointer for a function that returns a bool and
    // takes a T reference (any type of event)
    using EventFn = std::function<bool(T&)>;

   public:
    explicit EventDispatcher(Event& event) : event(event) {}

    template <EventInstanceType T>
    // Call
    bool Dispatch(EventFn<T> func) {
        // Check for type of event
        if (event.getType() == T::getStaticType()) {
            // Call the event handling function
            event.event_handled = func(*(T*)&event);
            return true;
        }
        return false;
    }

   private:
    Event& event;
};
*/

}  // namespace Pr
