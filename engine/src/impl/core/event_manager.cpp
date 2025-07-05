module presto.core.event_manager;

import presto.internal.managers;

namespace Pr {

EventManager& EventManager::Get() { return EventManagerImpl::get(); };

}  // namespace Pr
