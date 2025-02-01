#pragma once

#include "Event.h"

namespace Presto {
    class ObjectEvent : public Event {
       public:
        [[nodiscard]] void* getPtr() const { return this->_objectPtr; }

        EVENT_CLASS_CATEGORY(EventCategory::EventCategoryObject)

       protected:
        // Constructor
        template <typename T>
        explicit ObjectEvent(T* objectPtr) : _objectPtr((void*)(objectPtr)) {}
        void* _objectPtr;
    };

    class ObjectCreatedEvent : public ObjectEvent {
       public:
        EVENT_CLASS_TYPE(ObjectCreated)

        template <typename T>
        explicit ObjectCreatedEvent(T* objectPtr) : ObjectEvent(objectPtr) {}

        [[nodiscard]] std::string toString() const override {
            return std::format("Entity created at address 0x{}", _objectPtr);
        }
    };

    class ObjectDestroyedEvent : public ObjectEvent {
       public:
        EVENT_CLASS_TYPE(ObjectDestroyed)

        template <typename T>
        explicit ObjectDestroyedEvent(T* objectPtr) : ObjectEvent(objectPtr) {}

        [[nodiscard]] std::string toString() const override {
            return std::format("Entity destroyed at address 0x{}", _objectPtr);
        }
    };
}  // namespace Presto
