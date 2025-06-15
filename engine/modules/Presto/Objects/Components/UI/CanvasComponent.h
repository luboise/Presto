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

    CanvasGroup* group(Presto::size_t index = 0);

    /**
     * @brief  Creates a new canvas group and returns it. The new canvas group
     * can be configured by chaining calls together.
     *
     * For example, canvas.newGroup().addItem({...}).addItem({...})
     */
    CanvasGroup& newGroup();

    [[nodiscard]] VisualExtents size() const;
    void setSize(VisualExtents);

   private:
    CanvasComponent();

    VisualExtents size_{.width = 1, .height = 1};
    std::vector<CanvasGroup> groups_;
};
}  // namespace Presto
