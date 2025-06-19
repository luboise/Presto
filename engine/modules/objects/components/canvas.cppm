export module presto.objects.components.canvas;
export import :canvasitems;

import presto.core;

export namespace Presto {

/*
struct CanvasDrawDetails;
struct UniformBuffer;
struct CanvasItemAttributes;
*/

class PRESTO_API CanvasItem : LazyCalculator {
    friend class RenderingManager;

   public:
    CanvasItem();
    explicit CanvasItem(CanvasPosition position);
    explicit CanvasItem(CanvasItemAttributes attributes);

    virtual ~CanvasItem();

    [[nodiscard]] UniformBuffer& buffer();

    [[nodiscard]] CanvasItemAttributes attributes() const;
    CanvasItem& setAttributes(CanvasItemAttributes attributes);

    CanvasItem& setOpacity(Presto::float32_t);

    [[nodiscard]] const CanvasPosition& position() const;
    [[nodiscard]] const TexturePtr& texture() const;

    CanvasItem& setTexture(const TexturePtr&);

    CanvasItem(CanvasItem&&) noexcept;

    CanvasItem(const CanvasItem&) = delete;
    CanvasItem& operator=(const CanvasItem&) = delete;
    CanvasItem& operator=(CanvasItem&&) noexcept;

   protected:
    // void setDrawDetails(CanvasDrawDetails);
    // [[nodiscard]] virtual const CanvasDrawDetails& drawDetails() const;

    void setMeshId(mesh_registration_id_t);
    [[nodiscard]] mesh_registration_id_t meshId() const;

   private:
    struct Impl;
    Allocated<Impl> impl_;
};

class PRESTO_API CanvasGroup {
    friend class CanvasComponent;
    friend class RenderingManager;

   public:
    ~CanvasGroup() = default;

    void addItem(CanvasItem item) { items_.emplace_back(std::move(item)); };
    void setItem(Presto::size_t index, CanvasItem item) {
        items_[index] = std::move(item);
    }

    [[nodiscard]] const auto& items() const { return this->items_; }

    CanvasGroup(const CanvasGroup&) = default;
    CanvasGroup(CanvasGroup&&) = default;

    CanvasGroup& operator=(const CanvasGroup&) = delete;
    CanvasGroup& operator=(CanvasGroup&&) = delete;

   private:
    CanvasGroup() = default;

    std::vector<CanvasItem> items_;
};

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

};  // namespace Presto

#include "Objects/Components/UI/CanvasButton.h"  // IWYU pragma: export
#include "Objects/Components/UI/CanvasGroup.h"   // IWYU pragma: export
#include "Objects/Components/UI/CanvasItem.h"    // IWYU pragma: export
#include "Objects/Components/UI/CanvasRect.h"    // IWYU pragma: export
