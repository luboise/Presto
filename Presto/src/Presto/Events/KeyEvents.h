#pragma once

#include "Event.h"

namespace Presto {
    // Intended to be abstract
    class PRESTO_API KeyEvent : public Event {
       public:
        inline int GetKeyCode() const { return this->keyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

       protected:
        // Constructor
        KeyEvent(int keycode) { this->keyCode = keycode; }
        int keyCode;
    };

    class PRESTO_API KeyPressedEvent : public KeyEvent {
       public:
        KeyPressedEvent(int keycode, int repeatcount) : KeyEvent(keycode) {
            this->repeatCount = repeatcount;
        }

        inline int getRepeatCount() const { return this->repeatCount; }

        std::string ToString() const {
            std::stringstream ss;
            ss << "KeyPressedEvent: Key " << this->keyCode << "  ("
               << this->repeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

       private:
        int repeatCount;
    };

    class PRESTO_API KeyReleasedEvent : public KeyEvent {
       public:
        // Call parent constructor
        KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

        std::string ToString() const {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << this->keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };
}  // namespace Presto