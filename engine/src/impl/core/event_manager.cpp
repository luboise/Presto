module presto.core.event_manager;
import presto.internal.eventimpl;

namespace Pr {
EventManager& EventManager::Get() { return EventManagerImpl::get(); };
}  // namespace Pr
