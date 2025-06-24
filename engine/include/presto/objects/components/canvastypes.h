#include "presto/core/types.h"

namespace Pr {

struct CanvasItemAttributes {
    Pr::vec2 position{0, 0};
    Pr::vec2 offset{0, 0};
    Pr::vec2 scale{1, 1};
    Pr::float32_t opacity{1};
    Pr::float32_t padding{0};
};

using CanvasUnits = Pr::float32_t;

constexpr CanvasUnits PR_CANVAS_MIN_VALUE = -1;
constexpr CanvasUnits PR_CANVAS_MAX_VALUE = 1;

using CanvasPosition = Pr::vec2;

/** @brief Clamps a canvas position to be within the canvas bounds.
 *  @see PR_CANVAS_MIN_VALUE and PR_CANVAS_MAX_VALUE
 */
CanvasPosition Clamped(const CanvasPosition& pos);

}  // namespace Pr
