#pragma once

#include "presto/core/platform.h"

#include "./canvas_base.h"
#include "presto/core.h"
#include "presto/math/shapes.h"

namespace Pr {

class ImageAsset;

class PRESTO_API CanvasButton : public CanvasItem {
   public:
    explicit CanvasButton(CanvasPosition position);

    void setImage(const ImagePtr&);

   private:
    ImagePtr image_;
};

struct Rectangle;

struct CanvasRectProps {
    Pr::vec2 pos{0, 0};
    Pr::float32_t width{2};
    Pr::float32_t height{1};
};

class CanvasRect final : public CanvasItem {
   public:
    explicit CanvasRect(CanvasRectProps);
    void setProps(CanvasRectProps);

    // [[nodiscard]] const Rectangle& rectangle() const;
    // void setRectangle(Rectangle rect);

   private:
    Rectangle rect_;
};

}  // namespace Pr
