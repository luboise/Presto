#pragma once

#include "Presto/Modules/ObjectsModule/Component/Component.h"

namespace Presto {
    typedef std::map<component_class_t, Component*> ComponentMap;

    class PRESTO_API Entity {
       public:
        Entity();
        virtual ~Entity();

        uint32_t getId() const;

        template <typename ComponentClass>
        void setComponent(ComponentClass* component_ptr) {
            component_class_t classID = typeid(ComponentClass).hash_code();
            _components.emplace(classID, component_ptr);
        }

       private:
        uint32_t _id;

        ComponentMap _components;

        static uint32_t getNextAvailableId(void);
        static uint32_t _currentId;
    };

    typedef Entity* entity_t;
}  // namespace Presto