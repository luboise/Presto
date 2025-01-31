#include "Presto/Components/UI/CanvasItem.h"

namespace Presto {
    [[nodiscard]] CanvasPosition CanvasPosition::clamped() const {
        return {.x = std::clamp(x, PR_CANVAS_MIN_VALUE, PR_CANVAS_MAX_VALUE),
                .y = std::clamp(y, PR_CANVAS_MIN_VALUE, PR_CANVAS_MAX_VALUE)};
    }

}  // namespace Presto
