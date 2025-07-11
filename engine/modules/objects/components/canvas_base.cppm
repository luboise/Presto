module;
#include "presto/platform.h"

export module presto.objects.components.canvas:base;

import std;

import :types;

import presto.objects.base;
import presto.core;
import presto.utils;
import presto.assets.mesh;

import presto.types;

import presto.rendering.buffer;
import presto.rendering.texture;

export namespace Pr {

/*
struct CanvasDrawDetails;
*/

/*
struct CanvasDrawDetails {
mesh_registration_id_t mesh;
TransformData transform;
};
*/

class PRESTO_API CanvasItem : LazyCalculator {
   public:
    CanvasItem();
    explicit CanvasItem(CanvasPosition position);
    explicit CanvasItem(CanvasItemAttributes attributes);

    virtual ~CanvasItem();

    [[nodiscard]] UniformBuffer& buffer();

    [[nodiscard]] CanvasItemAttributes attributes() const;
    CanvasItem& setAttributes(CanvasItemAttributes attributes);

    CanvasItem& setOpacity(Pr::float32_t);

    [[nodiscard]] const CanvasPosition& position() const;
    [[nodiscard]] const Pr::Ptr<Pr::Texture>& texture() const;

    CanvasItem& setTexture(const Pr::Ptr<Pr::Texture>&);

    CanvasItem(CanvasItem&&) noexcept;

    CanvasItem(const CanvasItem&) = delete;
    CanvasItem& operator=(const CanvasItem&) = delete;
    CanvasItem& operator=(CanvasItem&&) noexcept;

    [[nodiscard]] mesh_registration_id_t meshId() const;

   protected:
    void setMeshId(mesh_registration_id_t);

   private:
    struct Impl;
    Allocated<Impl> impl_;
};

class PRESTO_API CanvasGroup {
    friend class CanvasComponent;

   public:
    ~CanvasGroup() = default;

    void addItem(CanvasItem item) { items_.emplace_back(std::move(item)); };
    void setItem(Pr::size_t index, CanvasItem item) {
        items_[index] = std::move(item);
    }

    [[nodiscard]] std::vector<CanvasItem>& items() { return this->items_; }

    CanvasGroup(const CanvasGroup&) = default;
    CanvasGroup(CanvasGroup&&) = default;

    CanvasGroup& operator=(const CanvasGroup&) = delete;
    CanvasGroup& operator=(CanvasGroup&&) = delete;

   private:
    CanvasGroup() = default;

    std::vector<CanvasItem> items_;
};

class CanvasComponent : public Component {
    // friend class EntityManager;
    // friend class EntityManagerImpl;

   public:
    CanvasComponent();

    // ~CanvasComponent() override;
    CanvasGroup& addGroup(CanvasGroup = {});

    CanvasGroup* group(Pr::size_t groupIndex = 0);
    [[nodiscard]] std::vector<CanvasGroup>& groups();

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
    VisualExtents size_{.width = 1, .height = 1};
    std::vector<CanvasGroup> groups_;
};

};  // namespace Pr
