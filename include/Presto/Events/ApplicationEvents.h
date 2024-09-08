#pragma once

#include "Presto/Event.h"

#include <sstream>

namespace Presto {
    /**
     * WindowResizeEvent
     * - Created when window is resized.
     * - Has a width and height parameter (the new width and height of the
     * window)
     */
    class PRESTO_API WindowResizeEvent : public Event {
       public:
        WindowResizeEvent(unsigned new_width, unsigned new_height) {
            this->width = new_width;
            this->height = new_height;
        }

        inline unsigned GetWidth() const { return width; }
        inline unsigned GetHeight() const { return height; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << width << ", " << height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

       private:
        unsigned width, height;
    };

    class PRESTO_API WindowCloseEvent : public Event {
       public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class PRESTO_API AppTickEvent : public Event {
       public:
        AppTickEvent() {}

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class PRESTO_API AppUpdateEvent : public Event {
       public:
        AppUpdateEvent() {}

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class PRESTO_API AppRenderEvent : public Event {
       public:
        AppRenderEvent() {}

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}  // namespace Presto
