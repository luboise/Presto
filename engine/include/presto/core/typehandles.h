#include <memory>

#include "presto/core/concepts.h"

namespace Presto {

// Handle types
template <typename T>
using Allocated = std::unique_ptr<T>;
template <typename T>
using Ptr = std::shared_ptr<T>;
template <typename T>
using Ref = std::weak_ptr<T>;

}  // namespace Presto
