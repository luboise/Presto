#pragma once

#include "Event.h"

#include <sstream>

namespace Presto {
    /**
     * WindowResizeEvent
     * - Created when window is resized.
     * - Has a width and height parameter (the new width and height of the
     * window)
     */
    class WindowResizeEvent : public Event {
       public:
        WindowResizeEvent(unsigned new_width, unsigned new_height) {
            this->width = new_width;
            this->height = new_height;
        }

        [[nodiscard]] inline unsigned GetWidth() const { return width; }
        [[nodiscard]] inline unsigned GetHeight() const { return height; }

        [[nodiscard]] std::string toString() const override {
            return std::format("WindowResizeEvent: {}, {}", width, height);
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

       private:
        unsigned width, height;
    };

    class FramebufferResizedEvent : public Event {
       public:
        FramebufferResizedEvent(unsigned new_width, unsigned new_height)
            : width(new_width), height(new_height) {}

        [[nodiscard]] inline unsigned GetWidth() const { return width; }
        [[nodiscard]] inline unsigned GetHeight() const { return height; }

        [[nodiscard]] std::string toString() const override {
            return std::format("FramebufferResizedEvent: {}, {}", width,
                               height);
        }

        EVENT_CLASS_TYPE(FramebufferResized)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

       private:
        unsigned width, height;
    };

    class WindowCloseEvent : public Event {
       public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppTickEvent : public Event {
       public:
        AppTickEvent() {}

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppUpdateEvent : public Event {
       public:
        AppUpdateEvent() {}

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AppRenderEvent : public Event {
       public:
        AppRenderEvent() {}

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}  // namespace Presto
