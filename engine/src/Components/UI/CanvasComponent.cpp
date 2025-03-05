#include "Presto/Objects/Components/UI/CanvasComponent.h"

namespace Presto {

CanvasGroup& CanvasComponent::addGroup(CanvasGroup group) {
    CanvasGroup& new_group{groups_.emplace_back(group)};
    return new_group;
};

CanvasGroup& CanvasComponent::newGroup() {
    CanvasGroup& new_group{groups_.emplace_back(CanvasGroup{})};
    return new_group;
};

}  // namespace Presto
