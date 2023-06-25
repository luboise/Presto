#pragma once

#include "Event.h"

namespace Presto {
    class PRESTO_API MouseMovedEvent : public Event {
       public:
        MouseMovedEvent(float x, float y) {
            this->mouseX = x;
            this->mouseY = y;
        }

        inline float GetMouseX() const { return this->mouseX; }
        inline float GetMouseY() const { return this->mouseY; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: (" << this->mouseX << ", " << this->mouseY
               << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

       private:
        float mouseX;
        float mouseY;
    };

    class PRESTO_API MouseScrolledEvent : public Event {
       public:
        MouseScrolledEvent(float xOffset, float yOffset) {
            this->scrollXOffset = xOffset;
            this->scrollYOffset = yOffset;
        }

        inline float GetXOffset() const { return this->scrollXOffset; }
        inline float GetYOffset() const { return this->scrollYOffset; }

        std::string ToString() {
            std::stringstream ss;
            ss << "MouseScrolledEvent:  (x: " << scrollXOffset
               << ", y: " << scrollYOffset << ")";
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

       private:
        float scrollXOffset;
        float scrollYOffset;
    };

    class PRESTO_API MouseButtonEvent : public Event {
       public:
        inline int GetMouseButton() const { return mouse_button; }

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse |
                             EventCategoryMouseButton)
       protected:
        MouseButtonEvent(int button) { this->mouse_button = button; }
        int mouse_button;
    };

    class PRESTO_API MouseButtonPressedEvent : public MouseButtonEvent {
       public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        std::string ToString() {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent:  Button " << mouse_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseBtnPressed)
    };

    class PRESTO_API MouseButtonReleasedEvent : public MouseButtonEvent {
       public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        std::string ToString() {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent:  Button " << mouse_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseBtnReleased)
    };
}  // namespace Presto