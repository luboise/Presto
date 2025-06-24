#include "internal/managers/event_manager_impl.h"
#include "presto/core/event_manager.h"

namespace Pr {
EventManager& EventManager::Get() { return EventManagerImpl::get(); };
}  // namespace Pr
