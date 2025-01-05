#pragma once

#include "PrestoCore/Event.h"

namespace PrestoCore {
    // Intended to be abstract
    class KeyEvent : public Event {
       public:
        inline int GetKeyCode() const { return this->keyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

       protected:
        // Constructor
        KeyEvent(int keycode) { this->keyCode = keycode; }
        int keyCode;
    };

    class KeyPressedEvent : public KeyEvent {
       public:
        KeyPressedEvent(int keycode, int repeatcount) : KeyEvent(keycode) {
            this->repeatCount = repeatcount;
        }

        inline int getRepeatCount() const { return this->repeatCount; }

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: Key " << this->keyCode << "  ("
               << this->repeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

       private:
        int repeatCount;
    };

    class KeyReleasedEvent : public KeyEvent {
       public:
        // Call parent constructor
        KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << this->keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };
}  // namespace PrestoCore
