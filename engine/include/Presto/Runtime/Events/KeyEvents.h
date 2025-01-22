#pragma once

#include "Event.h"

namespace Presto {

    using KeyCode = Input::Key;

    // Intended to be abstract
    class KeyEvent : public Event {
       public:
        [[nodiscard]] inline KeyCode GetKeyCode() const {
            return this->keyCode;
        }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

       protected:
        // Constructor
        KeyEvent(KeyCode keycode) : keyCode(keycode) {}
        KeyCode keyCode;
    };

    class KeyPressedEvent : public KeyEvent {
       public:
        KeyPressedEvent(KeyCode keycode, int repeatcount)
            : KeyEvent(keycode), repeatCount(repeatcount) {}

        [[nodiscard]] inline int getRepeatCount() const {
            return this->repeatCount;
        }

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: Key " << Input::CodeOf(this->keyCode)
               << "  (" << this->repeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

       private:
        int repeatCount;
    };

    class KeyReleasedEvent : public KeyEvent {
       public:
        // Call parent constructor
        explicit KeyReleasedEvent(KeyCode keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << Input::CodeOf(this->keyCode);
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };
}  // namespace Presto
