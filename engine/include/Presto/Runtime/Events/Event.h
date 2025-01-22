#pragma once

#include "Presto/Core.h"

#include <functional>
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

        Key,

        MouseBtnPressed,
        MouseBtnReleased,
        MouseMoved,
        MouseScrolled,

        ObjectCreated,
        ObjectDestroyed,
    };

    enum EventCategory {
        None = 0x0000,
        EventCategoryApplication = 0x0001,

        EventCategoryInput = 0x0002,
        EventCategoryGamepad = 0x0003,
        EventCategoryKeyboard = 0x0004,
        EventCategoryMouse = 0x0005,
        EventCategoryMouseButton = 0x0006,

        EventCategoryObject = 0x0007
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

    class Event {
        // EventDispatcher can view private members, needed to dispatch the
        // events
        friend class EventDispatcher;

       public:
        [[nodiscard]] virtual EventType GetEventType() const = 0;
        [[nodiscard]] virtual const char* GetName() const = 0;
        [[nodiscard]] virtual int GetCategoryFlags() const = 0;
        [[nodiscard]] virtual std::string ToString() const {
            return GetName();
        };

        inline bool IsInCategory(EventCategory category) {
            return (GetCategoryFlags() & category);
        }

       protected:
        bool event_handled = false;
    };

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
