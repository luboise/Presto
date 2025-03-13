#pragma once

#include "Presto/Platform.h"

#include "Presto/Types/ObjectTypes.h"

namespace Presto {
// using component_class_t = std::uint32_t;

using class_id_t = size_t;

template <typename T>
const class_id_t ClassID = typeid(T).hash_code();

class PRESTO_API Component {
    using ComponentIDBit = std::uint32_t;

    friend class EntityManager;
    friend class EntityManagerImpl;

    static constexpr auto UNASSIGNED_ID{static_cast<component_id_t>(-1)};

   public:
    // [[nodiscard]] component_class_t getClassID() const {};

    template <ComponentType SubClass>
    bool isOfType() {
        return dynamic_cast<SubClass*>(this) != nullptr;
    }

    /*
template <ComponentType SubClass>
ComponentPtr<SubClass> as() {
return ComponentPtr<SubClass>{dynamic_cast<SubClass*>(this)};
}
    */

    [[nodiscard]] component_id_t id() const { return this->id_; };

   protected:
    Component() = default;
    virtual ~Component() = default;

   private:
    virtual void onEnterScene() {};

    component_id_t id_{UNASSIGNED_ID};
    bool renderable_ = false;
};

using GenericComponentPtr = ComponentPtr<Component>;

}  // namespace Presto
