#include "Presto/Components/UI/CanvasComponent.h"

namespace Presto {
    CanvasGroup& CanvasComponent::addGroup(CanvasGroup group) {
        CanvasGroup& new_group{groups_.emplace_back(group)};
        return new_group;
    };
}  // namespace Presto
