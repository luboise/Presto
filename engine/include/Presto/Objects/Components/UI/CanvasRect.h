#pragma once

#include "CanvasItem.h"
#include "Presto/Collision/Shapes.h"

namespace Presto {

struct Rectangle;

struct CanvasRectProps {
    Presto::vec2 pos{0, 0};
    Presto::float32_t width{2};
    Presto::float32_t height{1};
};

class CanvasRect final : public CanvasItem {
   public:
    explicit CanvasRect(CanvasRectProps);
    void setProps(CanvasRectProps);

    [[nodiscard]] const Rectangle& rectangle() const;
    void setRectangle(Rectangle rect);

   private:
    Rectangle rect_;
};

}  // namespace Presto
