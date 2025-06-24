#include "presto/objects/components/canvas.h"
import :types;

// export module presto.objects.components:canvas;

[[nodiscard]] Pr::CanvasPosition Pr::Clamped(const CanvasPosition& position) {
    using namespace Pr;

    return {std::clamp(position.x, PR_CANVAS_MIN_VALUE, PR_CANVAS_MAX_VALUE),
            std::clamp(position.y, PR_CANVAS_MIN_VALUE, PR_CANVAS_MAX_VALUE)};
}
