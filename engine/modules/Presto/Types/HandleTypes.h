#pragma once

#include <memory>
#include "Presto/Core/Concepts.h"

namespace Presto {

// Handle types
template <typename T>
using Allocated = std::unique_ptr<T>;
template <typename T>
using Ptr = std::shared_ptr<T>;
template <typename T>
using Ref = std::weak_ptr<T>;

class Asset;
template <DerivedFrom<Asset> T>
using AssetPtr = Ptr<T>;
}  // namespace Presto
