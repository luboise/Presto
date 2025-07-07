module;
#include "presto/platform.h"

export module presto.objects.components.canvas:canvas_items;
import :base;

import presto.math;

import presto.assets.image;

import presto.core;

export namespace Pr {

class PRESTO_API CanvasButton : public CanvasItem {
   public:
    explicit CanvasButton(CanvasPosition position);

    void setImage(const Pr::Ptr<Pr::ImageAsset>&);

   private:
    Pr::Ptr<Pr::ImageAsset> image_;
};

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
