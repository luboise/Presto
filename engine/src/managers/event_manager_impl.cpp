module presto.internal.managers.event_manager_impl;

import std;

import presto.objects.components;

namespace Pr {

using ConductorPtr = ComponentPtr<ConductorComponent>;

using MakeConductor = std::function<ConductorPtr(GenericComponentPtr& val)>;

EventManagerImpl::EventManagerImpl() = default;

void EventManagerImpl::registerCallbacks(Entity* entity) {
    for (const Ptr<ConductorComponent>& conductor : Pr::GetConductors(entity)) {
        if (conductor->registered_) {
            Pr::Assert(entity == conductor->entity,
                       "A Conductor component has been assigned to multiple "
                       "different entities.");

            continue;
        }

        /*
if (conductor->handlesKeyEvents_) {
    this->addHandler<KeyEvent>(
        [conductor](KeyEvent& event) { conductor->on(event); });
}
        */
        conductor->registered_ = true;
        conductor->entity = entity;
    }
}
}  // namespace Pr
