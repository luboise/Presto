#pragma once

#include "Presto/Runtime/Events/Event.h"
#include "Presto/Utils/TypeMap.h"

namespace Presto {

class PRESTO_API EventManager {
    template <typename... Args>
    using HandlerFunction = std::function<void(Args...)>;

   public:
    static EventManager& Get();

    template <typename E>
    // requires DerivedFrom<E, Event>
    void addHandler(EventHandler<E> handler) {
        std::vector<EventHandler<E>>& handlers{
            handlerMap_.get<EventHandler<E>>()};

        handlers.push_back(handler);
    }

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

   protected:
    TypeMap handlerMap_;

    template <typename E>
    void dispatchInternal(E& event);
};

}  // namespace Presto
