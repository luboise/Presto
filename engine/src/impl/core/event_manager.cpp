module presto.core.event_manager;
import presto.internal.event_manager;

namespace Pr {
EventManager& EventManager::Get() { return EventManagerImpl::get(); };
}  // namespace Pr
