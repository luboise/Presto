#pragma once

#include "Presto/Objects/Component.h"

#include "CanvasGroup.h"

namespace Presto {
class CanvasComponent : public Component {
    friend class EntityManagerImpl;

   public:
    // ~CanvasComponent() override;
    CanvasGroup& addGroup(CanvasGroup = {});

   private:
    CanvasComponent();

    std::vector<CanvasGroup> groups_;
};
}  // namespace Presto
