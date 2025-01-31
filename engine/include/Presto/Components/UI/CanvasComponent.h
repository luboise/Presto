#pragma once

#include "Presto/Objects/Component.h"

#include "Presto/Components/UI/CanvasGroup.h"

namespace Presto {
    class CanvasComponent : public Component {
        friend class EntityManager;

       public:
        // ~CanvasComponent() override;
        CanvasGroup& addGroup(CanvasGroup = {});

       private:
        CanvasComponent();

        std::vector<CanvasGroup> groups_;
    };
}  // namespace Presto
