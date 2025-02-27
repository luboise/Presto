#pragma once

/*
#include "Objects/Component.h"  // IWYU pragma: export
#include "Objects/Scene.h"      // IWYU pragma: export
#include "Objects/System.h"     // IWYU pragma: export
*/

#include "Presto/Objects/EntityManager.h"  // IWYU pragma: export

#include "Presto/Core/Concepts.h"
#include "Presto/Objects/Components.h"  // IWYU pragma: export
#include "Presto/Objects/Entity.h"      // IWYU pragma: export

#include "Presto/Types/ComponentTypes.h"  // IWYU pragma: export

namespace Presto {

/**
 * @brief  Creates a new component of any type. The arguments given must match
 * the arguments of the private constructor of that component.
 */
template <DerivedFrom<ConductorComponent> T, typename... Args>
ComponentPtr<T> NewComponent(Args... args) {
    return EntityManager::Get().newComponent<T>(args...);
};

namespace CreateComponent {
ComponentPtr<ModelComponent> Model(const ModelPtr& ptr);

template <DerivedFrom<ConductorComponent> T, typename... Args>
ComponentPtr<T> Conductor(Args... args) {
    return EntityManager::Get().newComponent<T>(args...);
};

}  // namespace CreateComponent

}  // namespace Presto
