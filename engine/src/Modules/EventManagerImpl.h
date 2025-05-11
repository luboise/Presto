#pragma once

#include "Modules/Module.h"

#include "Presto/Objects/Entity.h"
#include "Presto/Runtime/EventManager.h"

namespace Presto {

class EventManagerImpl final : public Module<EventManagerImpl>,
                               public EventManager {
    MODULE_FUNCTIONS(EventManagerImpl);

    friend void Entity::checkNewComponent(GenericComponentPtr);

   public:
    ~EventManagerImpl() override = default;

    void update() override {};
    void registerCallbacks(Entity*);

    template <typename E>
    void dispatchInternal(E& event) {
        handle(event);
    };

   private:
    EventManagerImpl();
};

}  // namespace Presto
