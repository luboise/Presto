#pragma once

#include "Presto/Core/Constants.h"

namespace Presto {

class Entity;
using EntityPtr = Ptr<Entity>;
using EntityRef = Ref<Entity>;

using component_id_t = PR_NUMERIC_ID;

class Component;

template <typename T>
concept ComponentType = std::derived_from<T, Component>;

template <ComponentType T>
using ComponentPtr = Ptr<T>;

template <ComponentType T>
using ComponentRef = Ref<T>;

}  // namespace Presto
