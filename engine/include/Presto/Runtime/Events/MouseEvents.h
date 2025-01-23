#pragma once

#include "Event.h"

namespace Presto {
    class MouseMovedEvent : public Event {
       public:
        MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {}

        [[nodiscard]] inline float GetMouseX() const { return this->mouseX; }
        [[nodiscard]] inline float GetMouseY() const { return this->mouseY; }

        [[nodiscard]] std::string toString() const override {
            return std::format("MouseMovedEvent: ({}, {})", this->mouseY,
                               this->mouseX);
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

       private:
        float mouseX;
        float mouseY;
    };

    class MouseScrolledEvent : public Event {
       public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : scrollXOffset(xOffset), scrollYOffset(yOffset) {}

        [[nodiscard]] inline float GetXOffset() const {
            return this->scrollXOffset;
        }
        [[nodiscard]] inline float GetYOffset() const {
            return this->scrollYOffset;
        }

        [[nodiscard]] std::string toString() const override {
            return std::format("MouseScrolledEvent:  (x: {}, y: {})",
                               scrollXOffset, scrollYOffset);
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

       private:
        float scrollXOffset;
        float scrollYOffset;
    };

    class MouseButtonEvent : public Event {
       public:
        [[nodiscard]] inline int GetMouseButton() const { return mouse_button; }

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse |
                             EventCategoryMouseButton)
       protected:
        MouseButtonEvent(int button) { this->mouse_button = button; }
        int mouse_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
       public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        [[nodiscard]] std::string toString() const override {
            return std::format("MouseButtonPressedEvent:  Button {}",
                               mouse_button);
        }

        EVENT_CLASS_TYPE(MouseBtnPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
       public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        [[nodiscard]] std::string toString() const override {
            return std::format("MouseButtonReleasedEvent:  Button {}",
                               mouse_button);
        }

        EVENT_CLASS_TYPE(MouseBtnReleased)
    };
}  // namespace Presto
