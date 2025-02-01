#pragma once

#include "Event.h"

namespace Presto {

    using KeyCode = Input::Key;

    // Intended to be abstract
    class KeyEvent : public Event {
       public:
        KeyEvent(const KeyEvent &) = default;
        KeyEvent(KeyEvent &&) = delete;
        KeyEvent &operator=(const KeyEvent &) = default;
        KeyEvent &operator=(KeyEvent &&) = delete;

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

        [[nodiscard]] std::string toString() const override {
            return std::format("KeyEvent: Key ", Input::CodeOf(this->keyCode_));
        }

        KeyEvent(KeyCode keycode, KeyEventType eventType)
            : eventType_(eventType), keyCode_(keycode) {}

       private:
        // Constructor

        KeyEventType eventType_;
        KeyCode keyCode_;
    };
}  // namespace Presto
