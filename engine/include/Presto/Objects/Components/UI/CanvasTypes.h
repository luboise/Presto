#pragma once

namespace Presto {

struct CanvasItemAttributes {
    Presto::vec2 position{0, 0};
    Presto::vec2 offset{0, 0};
    Presto::vec2 scale{1, 1};
    std::array<float, 2> padding{0, 0};
};

using CanvasUnits = Presto::float32_t;

constexpr CanvasUnits PR_CANVAS_MIN_VALUE = -1;
constexpr CanvasUnits PR_CANVAS_MAX_VALUE = 1;

using CanvasPosition = Presto::vec2;

/** @brief Clamps a canvas position to be within the canvas bounds.
 *  @see PR_CANVAS_MIN_VALUE and PR_CANVAS_MAX_VALUE
 */
CanvasPosition Clamped(const CanvasPosition& pos);

};  // namespace Presto
