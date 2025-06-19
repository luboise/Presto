#include <memory>

#include "Presto/Collision/Shapes.h"
#include "Presto/Core/Constants.h"

#include "Presto/Objects/Components/UI/CanvasItem.h"
#include "Presto/Objects/Components/UI/CanvasRect.h"
#include "Presto/Objects/Components/UI/CanvasTypes.h"

#include "Rendering/StructBuffer.h"

[[nodiscard]] Presto::CanvasPosition Presto::Clamped(
    const CanvasPosition& position) {
    using namespace Presto;

    return {std::clamp(position.x, PR_CANVAS_MIN_VALUE, PR_CANVAS_MAX_VALUE),
            std::clamp(position.y, PR_CANVAS_MIN_VALUE, PR_CANVAS_MAX_VALUE)};
}

namespace Presto {}  // namespace Presto
