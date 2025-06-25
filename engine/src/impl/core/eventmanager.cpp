module presto.core.eventmanager;
import presto.internal.eventimpl;

namespace Pr {
EventManager& EventManager::Get() { return EventManagerImpl::get(); };
}  // namespace Pr
