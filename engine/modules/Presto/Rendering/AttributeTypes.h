#pragma once

#include "Presto/Types/CoreTypeTraits.h"
#include "Presto/Types/CoreTypes.h"

namespace Presto {

using attribute_size_t = Presto::size_t;

struct BaseAttributeTypeDetails {
    Presto::size_t subtype_size;
    Presto::size_t count;
    Presto::size_t size;
};

template <typename T>
// TODO: Fix this constraint
// requires requires { SubTypeDetails<T>::subtype; }
struct AttributeTypeDetails : BaseAttributeTypeDetails {
    using details = SubTypeDetails<T>;
    using subtype = details::subtype;

    static constexpr Presto::size_t subtype_size = sizeof(subtype);
    static constexpr Presto::size_t count{details::subtype_count};
    static constexpr Presto::size_t size = subtype_size * count;
};

}  // namespace Presto
