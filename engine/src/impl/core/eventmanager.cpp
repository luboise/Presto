module presto.core.eventmanager;
import presto.internal.eventimpl;

namespace Presto {
EventManager& EventManager::Get() { return EventManagerImpl::get(); };
}  // namespace Presto
