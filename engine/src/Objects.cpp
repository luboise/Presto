#include "Presto/Objects.h"

namespace Presto {
// Define the specialization
template <>
ComponentPtr<ModelComponent> NewComponent<ModelComponent>(ModelPtr&& asset) {
    return std::make_shared<ModelComponent>(std::move(asset));
}

}  // namespace Presto
