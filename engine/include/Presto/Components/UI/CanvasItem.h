#pragma once

namespace Presto {

    using CanvasUnits = float;

    constexpr CanvasUnits PR_CANVAS_MIN_VALUE = 0;
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
        explicit CanvasItem(CanvasPosition position) : position(position) {}

       private:
        CanvasPosition position;
    };
}  // namespace Presto
