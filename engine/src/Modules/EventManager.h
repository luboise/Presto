#pragma once

#include "Modules/Module.h"

#include "Presto/Core/Concepts.h"
#include "Presto/Objects/Entity.h"

#include "Presto/Utils/TypeMap.h"

namespace Presto {

template <typename T>
    requires DerivedFrom<T, Event>
using EventHandler = std::function<void(T&)>;

class PRESTO_API EventManager final : public Module<EventManager> {
    MODULE_FUNCTIONS(EventManager);

    template <typename... Args>
    using HandlerFunction = std::function<void(Args...)>;

    friend void Entity::checkNewComponent(GenericComponentPtr);

   public:
    void update() override {}

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

    template <typename E>
    // requires DerivedFrom<E, Event>
    void addHandler(EventHandler<E> handler) {
        std::vector<EventHandler<E>>& handlers{
            handlerMap_.get<EventHandler<E>>()};

        handlers.push_back(handler);
    }

   protected:

   private:
    EventManager() = default;
    ~EventManager() override = default;

    void registerCallbacks(Entity*);

    // std::vector<HandlerFunction<>> eventHandlers_;
    // std::vector<KeyEventHandler> keyEventHandlers_;

    TypeMap handlerMap_;
};

}  // namespace Presto
