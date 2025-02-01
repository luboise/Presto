#pragma once

#include "Presto/Modules/Module.h"

#include "Presto/Objects/Entity.h"

#include "Presto/Runtime/Events/KeyEvents.h"

namespace Presto {

    class PRESTO_API EventManager final : public Module<EventManager> {
        MODULE_FUNCTIONS(EventManager);

        template <typename... Args>
        using HandlerFunction = std::function<void(Args...)>;

        friend void Entity::checkNewComponent(GenericComponentPtr);

       public:
        void update() override {}

        template <typename T>
        void addHandler(const HandlerFunction<T>& handler) {
            keyEventHandlers_.push_back(handler);
        };

        using KeyEventHandler = std::function<void(KeyEvent&)>;
        void addHandler(const KeyEventHandler& handler) {
            keyEventHandlers_.push_back(handler);
        }

        void onKeyEvent(KeyEvent& event);

       protected:
       private:
        EventManager() = default;
        ~EventManager() = default;

        void registerCallbacks(Entity&);

        // std::vector<HandlerFunction<>> eventHandlers_;
        std::vector<KeyEventHandler> keyEventHandlers_;
    };
}  // namespace Presto
