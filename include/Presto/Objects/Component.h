#pragma once

#include <cstdint>

#include "Presto/Core/Constants.h"
#include "Presto/Platform.h"

using component_class_t = uint32_t;

#define CLASS_ID(ClassName) (typeid(ClassName).hash_code())

namespace Presto {
    using component_id_t = PR_NUMERIC_ID;

    class PRESTO_API Component {
        friend class EntityManager;

        static constexpr auto UNASSIGNED_ID{static_cast<component_id_t>(-1)};

       public:
        Component() = default;
        virtual ~Component() = default;

        // [[nodiscard]] component_class_t getClassID() const {};

        template <typename Class>
        bool isOfType() {
            return dynamic_cast<Class>(this) != nullptr;
        }

        [[nodiscard]] component_id_t getId() const { return this->id_; };

       private:
        component_id_t id_{UNASSIGNED_ID};
        bool renderable_ = false;
    };

    using component_ptr = Component*;
}  // namespace Presto
