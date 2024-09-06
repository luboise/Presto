#pragma once

#include "Presto/Events/ObjectEvents.h"
#include "Presto/Modules/ObjectsModule/Component/Component.h"

namespace Presto {
    // Forward declaration
    class EntityManager;

    typedef std::map<component_class_t, Component*> ComponentMap;
    typedef uint32_t entity_id_t;

    class PRESTO_API Entity {
        friend class EntityManager;

       public:
        entity_id_t getId() const;

        template <typename ComponentClass>
        void setComponent(ComponentClass* component_ptr) {
            component_class_t classID = typeid(ComponentClass).hash_code();
            _components.emplace(classID, component_ptr);
        }

        ComponentMap getComponents() { return _components; }

       private:
        Entity(entity_id_t id);
        virtual ~Entity();

        entity_id_t _id;
        ComponentMap _components;
    };

    typedef Entity* entity_t;
}  // namespace Presto