#pragma once

#include "PrestoCore/Event.h"

namespace Presto {
    class PRESTO_API ObjectEvent : public Event {
       public:
        inline void* getPtr() const { return this->_objectPtr; }

        EVENT_CLASS_CATEGORY(EventCategoryObject)

       protected:
        // Constructor
        template <typename T>
        ObjectEvent(T* objectPtr) {
            this->_objectPtr = (void*)(objectPtr);
        }
        void* _objectPtr;
    };

    class PRESTO_API ObjectCreatedEvent : public ObjectEvent {
       public:
        EVENT_CLASS_TYPE(ObjectCreated)

        template <typename T>
        ObjectCreatedEvent(T* objectPtr) : ObjectEvent(objectPtr) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "Entity created at address 0x" << _objectPtr << ".";
            return ss.str();
        }
    };

    class PRESTO_API ObjectDestroyedEvent : public ObjectEvent {
       public:
        EVENT_CLASS_TYPE(ObjectDestroyed)

        template <typename T>
        ObjectDestroyedEvent(T* objectPtr) : ObjectEvent(objectPtr) {}

        std::string ToString() const override {
            std::stringstream ss;
            ss << "Entity at address 0x" << _objectPtr << " destroyed.";
            return ss.str();
        }
    };
}  // namespace Presto
