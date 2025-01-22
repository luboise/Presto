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
        virtual ~KeyEvent() = default;

        enum class KeyEventType { PRESSED, RELEASED };

        [[nodiscard]] inline KeyCode getKey() const { return this->keyCode_; }

        [[nodiscard]] inline KeyEventType type() const {
            return this->eventType_;
        }

        [[nodiscard]] inline bool isPressedEvent() const {
            return eventType_ == KeyEventType::PRESSED;
        }

        [[nodiscard]] inline bool isReleasedEvent() const {
            return eventType_ == KeyEventType::RELEASED;
        }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
        EVENT_CLASS_TYPE(Key)

        [[nodiscard]] std::string ToString() const override {
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
