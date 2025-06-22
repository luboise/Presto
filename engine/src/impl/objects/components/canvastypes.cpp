module presto.objects.components.canvas;
import :types;

// export module presto.objects.components:canvas;

[[nodiscard]] Presto::CanvasPosition Presto::Clamped(
    const CanvasPosition& position) {
    using namespace Presto;

    return {std::clamp(position.x, PR_CANVAS_MIN_VALUE, PR_CANVAS_MAX_VALUE),
            std::clamp(position.y, PR_CANVAS_MIN_VALUE, PR_CANVAS_MAX_VALUE)};
}
