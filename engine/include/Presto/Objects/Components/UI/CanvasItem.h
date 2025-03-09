#pragma once

#include "Presto/Aliases/Handles.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Types/CoreTypes.h"

#include "Presto/Utils/LazyCalculator.h"

namespace Presto {

class PRESTO_API CanvasItem : LazyCalculator {
   public:
    explicit CanvasItem(CanvasPosition position);
    ~CanvasItem();

    [[nodiscard]] CanvasItem& setAttributes(CanvasItemAttributes attributes);

    [[nodiscard]] const CanvasPosition& position() const;

    [[nodiscard]] const TexturePtr& texture() const;
    CanvasItem& setTexture(const TexturePtr&);

    CanvasItem(CanvasItem&&) noexcept;

    CanvasItem(const CanvasItem&) = delete;
    CanvasItem& operator=(const CanvasItem&) = delete;
    CanvasItem& operator=(CanvasItem&&) = delete;

   private:
    struct Impl;
    Allocated<Impl> impl_;
};

}  // namespace Presto
