#pragma once

#include "Presto/Core.h"

#include <functional>
#include <memory>
#include <string>

namespace Presto {
    enum class EventType {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLoseFocus,
        WindowMoved,

        AppTick,
        AppUpdate,
        AppRender,

        GamepadBtnPressed,
        GamepadBtnReleased,

        KeyPressed,
        KeyReleased,

        MouseBtnPressed,
        MouseBtnReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory {
        None = 0,
        EventCategoryApplication = AS_BIT(1),
        EventCategoryInput = AS_BIT(2),
        EventCategoryGamepad = AS_BIT(3),
        EventCategoryKeyboard = AS_BIT(4),
        EventCategoryMouse = AS_BIT(5),
        EventCategoryMouseButton = AS_BIT(6)
    };

    // Defines that can be reused between child classes to get event types,
    // consistent between classes

#define EVENT_CLASS_TYPE(type)                                   \
    static EventType GetStaticType() { return EventType::type; } \
    virtual EventType GetEventType() const override {            \
        return GetStaticType();                                  \
    }                                                            \
    virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int GetCategoryFlags() const override { return category; }

    class PRESTO_API Event {
        // EventDispatcher can view private members, needed to dispatch the
        // events
        friend class EventDispatcher;

       public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); };

        inline bool IsInCategory(EventCategory category) {
            return (GetCategoryFlags() & category);
        }

       protected:
        bool event_handled = false;
    };

    // No PRESTO_API here as we don't want outsiders to have this
    class EventDispatcher {
        template <typename T>
        // Reserved function pointer for a function that returns a bool and
        // takes a T reference (any type of event)
        using EventFn = std::function<bool(T&)>;

       public:
        EventDispatcher(Event& event) : event(event) {}

        template <typename T>
        // Call
        bool Dispatch(EventFn<T> func) {
            // Check for type of event
            if (event.GetEventType() == T::GetStaticType()) {
                // Call the event handling function
                event.event_handled = func(*(T*)&event);
                return true;
            }
            return false;
        }

       private:
        Event& event;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& event) {
        return os << event.ToString();
    }
}  // namespace Presto