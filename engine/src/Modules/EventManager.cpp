#include "EventManagerImpl.h"

#include "Presto/Objects.h"
#include "Presto/Objects/Components/ConductorComponent.h"

namespace Presto {
using ConductorPtr = ComponentPtr<ConductorComponent>;

using MakeConductor = std::function<ConductorPtr(GenericComponentPtr& val)>;

void EventManagerImpl::registerCallbacks(Entity* entity) {
    for (const Ptr<ConductorComponent>& conductor : entity->getConductors()) {
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

EventManager& EventManager::Get() { return EventManagerImpl::get(); };

}  // namespace Presto
