module;
#include "presto/module.h"

export module presto.internal.managers.event_manager_impl;

import presto.objects.entity;
import presto.core.event;
import presto.core.event_manager;

import presto.internal.managers.manager;

export namespace Pr {

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

}  // namespace Pr
