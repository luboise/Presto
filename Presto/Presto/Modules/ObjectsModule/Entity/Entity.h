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
        void setComponent(ComponentClass* component) {
            component_class_t name = component->getClassID();
            _components.emplace(component.getClassID,
                                std::make_shared(component));
        }

       private:
        uint32_t _id;

        ComponentMap _components;

        static uint32_t getNextAvailableId(void);
        static uint32_t _currentId;
    };

    typedef Entity* entity_t;
}  // namespace Presto