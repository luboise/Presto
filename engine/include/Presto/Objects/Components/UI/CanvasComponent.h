#pragma once

#include "Presto/Objects/Component.h"

#include "CanvasGroup.h"

namespace Presto {
class CanvasComponent : public Component {
    friend class EntityManager;
    friend class EntityManagerImpl;
    friend class RenderingManager;

   public:
    // ~CanvasComponent() override;
    CanvasGroup& addGroup(CanvasGroup = {});

    /**
     * @brief  Creates a new canvas group and returns it. The new canvas group
     * can be configured by chaining calls together.
     *
     * For example, canvas.newGroup().addItem({...}).addItem({...})
     */
    CanvasGroup& newGroup();

   private:
    CanvasComponent() = default;

    std::vector<CanvasGroup> groups_;
};
}  // namespace Presto
