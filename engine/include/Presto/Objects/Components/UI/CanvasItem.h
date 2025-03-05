#pragma once

#include "Presto/Aliases/Handles.h"
#include "Presto/Types/CoreTypes.h"

namespace Presto {

using CanvasUnits = Presto::float32_t;

constexpr CanvasUnits PR_CANVAS_MIN_VALUE = -1;
constexpr CanvasUnits PR_CANVAS_MAX_VALUE = 1;

struct CanvasPosition {
    CanvasUnits x;
    CanvasUnits y;

    /** @brief Clamps a canvas position to be within the canvas bounds.
     *  @see PR_CANVAS_MIN_VALUE and PR_CANVAS_MAX_VALUE
     */
    [[nodiscard]] CanvasPosition clamped() const;
};

class PRESTO_API CanvasItem {
   public:
    explicit CanvasItem(CanvasPosition position) : position_(position) {}

    [[nodiscard]] const CanvasPosition& getPosition() const {
        return position_;
    }

    [[nodiscard]] const TexturePtr& texture() const;

    CanvasItem& setTexture(const TexturePtr&);

   private:
    CanvasPosition position_;
    TexturePtr texture_;
};

}  // namespace Presto
