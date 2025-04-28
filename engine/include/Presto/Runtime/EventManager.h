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

   protected:
    TypeMap handlerMap_;
};

}  // namespace Presto
