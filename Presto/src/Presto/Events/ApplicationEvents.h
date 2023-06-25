#pragma once

#include "Event.h"
#include "Presto/Core.h"

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
}  // namespace Presto