#include <format>
#include "presto/events.h"

#include "presto/core/event.h"
#include "presto/core/types.h"

#include ":keycodes.h"

namespace Pr {
using KeyCode = Input::Key;

// Intended to be abstract
class KeyEvent : public Event {
   public:
    KeyEvent(const KeyEvent&) = default;
    KeyEvent(KeyEvent&&) = delete;
    KeyEvent& operator=(const KeyEvent&) = default;
    KeyEvent& operator=(KeyEvent&&) = delete;

    ~KeyEvent() override = default;

    enum class KeyEventType : uint8_t { PRESSED, RELEASED };

    [[nodiscard]] KeyCode getKey() const { return this->keyCode_; }

    [[nodiscard]] bool isPressedEvent() const {
        return eventType_ == KeyEventType::PRESSED;
    }

    [[nodiscard]] bool isReleasedEvent() const {
        return eventType_ == KeyEventType::RELEASED;
    }

    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard |
                         EventCategory::EventCategoryInput)
    EVENT_CLASS_TYPE(Key)

    [[nodiscard]] Pr::string toString() const override {
        return std::format("KeyEvent: Key ", Input::CodeOf(this->keyCode_));
    }

    KeyEvent(KeyCode keycode, KeyEventType eventType)
        : eventType_(eventType), keyCode_(keycode) {}

   private:
    // Constructor

    KeyEventType eventType_;
    KeyCode keyCode_;
};

struct MousePosition {
    float x;
    float y;

    operator Pr::vec2() { return Pr::vec2{this->x, this->y}; }
};

class MouseEvent : public Event {
   public:
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryInput |
                         EventCategory::EventCategoryMouse)
};

class MouseMovedEvent : public MouseEvent {
   public:
    MouseMovedEvent(float x, float y) : position_({.x = x, .y = y}) {}

    [[nodiscard]] float mouseX() const { return position_.x; }
    [[nodiscard]] float mouseY() const { return position_.y; }

    // TODO: Fix these to come from the window and not include black bars rather
    // than from the framebuffer. The real issue is that its not mapping the
    // screen space to the user, but the window space
    [[nodiscard]] const auto& pos() const { return position_; }
    [[nodiscard]] MousePosition posNormalised() const;

    [[nodiscard]] Pr::string toString() const override {
        return std::format("MouseMovedEvent: ({}, {})", position_.x,
                           position_.y);
    }

    EVENT_CLASS_TYPE(MouseMoved)

   private:
    MousePosition position_;
};

class MouseScrolledEvent : public MouseEvent {
   public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : scrollXOffset(xOffset), scrollYOffset(yOffset) {}

    [[nodiscard]] float GetXOffset() const { return this->scrollXOffset; }
    [[nodiscard]] float GetYOffset() const { return this->scrollYOffset; }

    [[nodiscard]] Pr::string toString() const override {
        return std::format("MouseScrolledEvent:  (x: {}, y: {})", scrollXOffset,
                           scrollYOffset);
    }

    EVENT_CLASS_TYPE(MouseScrolled)

   private:
    float scrollXOffset;
    float scrollYOffset;
};

class MouseButtonEvent : public MouseEvent {
   public:
    [[nodiscard]] int button() const { return button_; }

    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryInput |
                         EventCategory::EventCategoryMouse |
                         EventCategory::EventCategoryMouseButton)

   protected:
    explicit MouseButtonEvent(int button) : button_(button) {}
    int button_;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
   public:
    explicit MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

    [[nodiscard]] Pr::string toString() const override {
        return std::format("MouseButtonPressedEvent:  Button {}", button_);
    }

    EVENT_CLASS_TYPE(MouseBtnPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
   public:
    explicit MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

    [[nodiscard]] Pr::string toString() const override {
        return std::format("MouseButtonReleasedEvent:  Button {}", button_);
    }

    EVENT_CLASS_TYPE(MouseBtnReleased)
};
}  // namespace Pr
