#pragma once

#include <cstdint>

#include "Presto/Core/Concepts.h"
#include "Presto/Core/Constants.h"
#include "Presto/Platform.h"

using component_class_t = uint32_t;

template <typename T>
const auto ClassID = typeid(T).hash_code();

namespace Presto {
    using component_id_t = PR_NUMERIC_ID;

    class PRESTO_API Component {
        using ComponentIDBit = uint32_t;

        friend class EntityManager;

        static constexpr auto UNASSIGNED_ID{static_cast<component_id_t>(-1)};

       public:
        // [[nodiscard]] component_class_t getClassID() const {};

        template <DerivedFrom<Component> SubClass>
        bool isOfType() {
            return dynamic_cast<SubClass*>(this) != nullptr;
        }

        [[nodiscard]] component_id_t getId() const { return this->id_; };

       private:
        // Component() = default;
        //  virtual ~Component() = default;

        virtual void onEnterScene() {};

        component_id_t id_{UNASSIGNED_ID};
        bool renderable_ = false;
    };

    template <DerivedFrom<Component> T = Component>
    using ComponentPtr = Ptr<T>;

    using GenericComponentPtr = ComponentPtr<Component>;

}  // namespace Presto
