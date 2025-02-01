#pragma once

#include "Event.h"

namespace Presto {
    /**
     * WindowResizeEvent
     * - Created when window is resized.
     * - Has a width and height parameter (the new width and height of the
     * window)
     */
    class WindowResizeEvent : public Event {
       public:
        WindowResizeEvent(unsigned new_width, unsigned new_height)
            : width(new_width), height(new_height) {}

        [[nodiscard]] unsigned GetWidth() const { return width; }
        [[nodiscard]] unsigned GetHeight() const { return height; }

        [[nodiscard]] std::string toString() const override {
            return std::format("WindowResizeEvent: {}, {}", width, height);
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)

       private:
        unsigned width, height;
    };

    class FramebufferResizedEvent : public Event {
       public:
        FramebufferResizedEvent(unsigned new_width, unsigned new_height)
            : width(new_width), height(new_height) {}

        [[nodiscard]] unsigned GetWidth() const { return width; }
        [[nodiscard]] unsigned GetHeight() const { return height; }

        [[nodiscard]] std::string toString() const override {
            return std::format("FramebufferResizedEvent: {}, {}", width,
                               height);
        }

        EVENT_CLASS_TYPE(FramebufferResized)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)

       private:
        unsigned width, height;
    };

    class WindowCloseEvent : public Event {
       public:
        WindowCloseEvent() = default;

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
    };

    class AppTickEvent : public Event {
       public:
        AppTickEvent() = default;

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
    };

    class AppUpdateEvent : public Event {
       public:
        AppUpdateEvent() = default;

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
    };

    class AppRenderEvent : public Event {
       public:
        AppRenderEvent() = default;

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
    };
}  // namespace Presto
