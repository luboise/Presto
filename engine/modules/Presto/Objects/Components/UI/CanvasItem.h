#pragma once

#include "Presto/Aliases/Handles.h"
#include "Presto/Core/Constants.h"
#include "Presto/Types/CoreTypes.h"
#include "Presto/Utils/LazyCalculator.h"

#include "Presto/Objects/Components/UI/CanvasTypes.h"

namespace Presto {

struct CanvasDrawDetails;
struct UniformBuffer;
struct CanvasItemAttributes;

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

}  // namespace Presto
