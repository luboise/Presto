#include "presto/core/eventmanager.h"
#include "presto/internal/eventimpl.h"

namespace Presto {
EventManager& EventManager::Get() { return EventManagerImpl::get(); };
}  // namespace Presto
