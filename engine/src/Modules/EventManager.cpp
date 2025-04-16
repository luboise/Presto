#include "EventManager.h"

#include <memory>

#include "Presto/Objects/Components/ConductorComponent.h"

#include "Presto/Objects.h"

namespace Presto {
using ConductorPtr = ComponentPtr<ConductorComponent>;

using MakeConductor = std::function<ConductorPtr(GenericComponentPtr& val)>;

void EventManager::registerCallbacks(Entity* entity) {
    for (const auto& conductor : entity->getConductors()) {
        if (conductor->registered_) {
            PR_ASSERT(entity == conductor->entity,
                      "A Conductor component has been assigned to multiple "
                      "different entities.");

            continue;
        }

        if (conductor->handlesKeyEvents_) {
            this->addHandler<KeyEvent>(
                [conductor](KeyEvent& event) { conductor->on(event); });
        }
        conductor->registered_ = true;
        conductor->entity = entity;
    }
}

}  // namespace Presto
