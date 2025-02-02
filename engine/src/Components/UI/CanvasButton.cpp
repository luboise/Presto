#include "Presto/Components/UI/CanvasButton.h"

namespace Presto {
    CanvasButton::CanvasButton(CanvasPosition position)
        : CanvasItem(position) {

          };

    void CanvasButton::setImage(const ImagePtr& image) { image_ = image; };
}  // namespace Presto
