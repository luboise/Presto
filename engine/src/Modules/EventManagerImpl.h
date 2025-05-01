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
        requires DerivedFrom<E, Event>
    void handle(E& event) {
        std::vector<EventHandler<E>>& handlers{
            handlerMap_.get<EventHandler<E>>()};
        for (const EventHandler<E>& handler : handlers) {
            try {
                handler(event);
            } catch (std::exception& e) {
                PR_ERROR("Error handling key event: {}", e.what());
            }
        }
    }

   private:
    EventManagerImpl();
};

}  // namespace Presto
