#include "EventManager.h"

#include "Presto/Components/ConductorComponent.h"
#include "Presto/Core/Logging.h"

#include "Presto/Objects.h"

namespace Presto {
    void EventManager::registerCallbacks(Entity& entity) {
        ComponentPtr conductor{entity.getComponent<ConductorComponent>()};

        if (conductor == nullptr) {
            PR_WARN("Attempted to register entity with no conductor.");
            return;
        }

        if (conductor->handlesInput_) {
            keyEventHandlers_.emplace_back(
                [conductor](KeyEvent& event) { conductor->onInput(event); });
        }

        conductor->entity = &entity;
    };

    void EventManager::onKeyEvent(KeyEvent& event) {
        for (const auto& handler : keyEventHandlers_) {
            handler(event);
        }
    };
}  // namespace Presto
