#pragma once

#include "Presto/Core/Concepts.h"
#include "Presto/Objects/Component.h"

namespace Presto {

template <typename T>
    requires DerivedFrom<T, Component, Strictness::STRICT>
struct Subcomponent {
    using super_t = T;
};

}  // namespace Presto
