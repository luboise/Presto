#include "EventManager.h"
#include <memory>

#include "Presto/Components/ConductorComponent.h"
#include "Presto/Core/Logging.h"

#include "Presto/Objects.h"

namespace Presto {
    void EventManager::registerCallbacks(Entity& entity) {
        for (auto& component : entity.components_ | std::views::values) {
            auto conductor{
                std::dynamic_pointer_cast<ConductorComponent>(component)};
            if (conductor == nullptr) {
                continue;
            }

            if (conductor->handlesInput_) {
                keyEventHandlers_.emplace_back([conductor](KeyEvent& event) {
                    conductor->onInput(event);
                });
            }

            conductor->entity = &entity;
        }
    }

    void EventManager::onKeyEvent(KeyEvent& event) {
        for (const auto& handler : keyEventHandlers_) {
            handler(event);
        }
    };
}  // namespace Presto
