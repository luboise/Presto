#pragma once

#include "Presto/Modules/Module.h"

#include "Presto/Objects/Entity.h"

#include "Presto/Runtime/Events/KeyEvents.h"

namespace Presto {
    class Entity;
    class Component;

    class PRESTO_API EventManager : public Module<EventManager> {
        template <typename... Args>
        using HandlerFunction = std::function<void(Args...)>;

        friend void Entity::checkNewComponent(GenericComponentPtr);

       public:
        static void init() {
            instance_ = std::unique_ptr<EventManager>(new EventManager());
        };
        void update() override {}
        static void shutdown() {};

        // Deleted functions
        EventManager(const EventManager&) = delete;
        EventManager(EventManager&&) = delete;
        EventManager& operator=(const EventManager&) = delete;
        EventManager& operator=(EventManager&&) = delete;

        template <typename T>
        void addHandler(const HandlerFunction<T>& handler) {
            keyEventHandlers_.push_back(handler);
        };

        using KeyEventHandler = std::function<void(KeyEvent&)>;
        void addHandler(const KeyEventHandler& handler) {
            keyEventHandlers_.push_back(handler);
        }

        void onKeyEvent(KeyEvent& event);

       private:
        EventManager() = default;

        void registerCallbacks(Entity&);

        // std::vector<HandlerFunction<>> eventHandlers_;
        std::vector<KeyEventHandler> keyEventHandlers_;
    };
}  // namespace Presto
