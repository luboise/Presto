module;
#include <concepts>
#include <typeinfo>

export module presto.component;

export import presto.core.platform;
// #include "presto/platform.h"

import presto.core.types;
import presto.core.concepts;

export namespace Presto {
// using component_class_t = Presto::uint32_t;

using class_id_t = Presto::size_t;

template <typename T>
const class_id_t ClassID = typeid(T).hash_code();

class PRESTO_API Component {
    friend class EntityManager;
    friend class EntityManagerImpl;

    using ComponentIDBit = Presto::uint32_t;

    static constexpr auto UNASSIGNED_ID{static_cast<component_id_t>(-1)};

   public:
    // [[nodiscard]] component_class_t getClassID() const {};

    template <ComponentType SubClass>
    bool isOfType() {
        return dynamic_cast<SubClass*>(this) != nullptr;
    }

    [[nodiscard]] bool enteredScene() const { return enteredScene_; }

    /*
template <ComponentType SubClass>
ComponentPtr<SubClass> as() {
return ComponentPtr<SubClass>{dynamic_cast<SubClass*>(this)};
}
    */

    [[nodiscard]] component_id_t id() const { return this->id_; };

    Component(const Component&) = default;
    Component(Component&&) = delete;
    Component& operator=(const Component&) = default;
    Component& operator=(Component&&) = delete;

   protected:
    Component() = default;
    virtual ~Component() = default;

   private:
    virtual void onEnterScene() {};

    bool enteredScene_{false};

    component_id_t id_{UNASSIGNED_ID};
    bool renderable_ = false;
};

template <typename T>
    requires DerivedFrom<T, Component, Strictness::STRICTLY_DERIVED>
struct Subcomponent {
    using super_t = T;
};

using GenericComponentPtr = ComponentPtr<Component>;

using component_id_t = PR_NUMERIC_ID;

template <typename T>
concept ComponentType = std::derived_from<T, Component>;

template <ComponentType T>
using ComponentPtr = Ptr<T>;

template <ComponentType T>
using ComponentRef = Ref<T>;

}  // namespace Presto
