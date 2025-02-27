#pragma once

#include "Presto/Assets/ImageAsset.h"

#include "Presto/Components/UI/CanvasItem.h"

namespace Presto {
class ImageAsset;

class PRESTO_API CanvasButton : public CanvasItem {
   public:
    explicit CanvasButton(CanvasPosition position);

    void setImage(const ImagePtr&);

   private:
    ImagePtr image_;
};

}  // namespace Presto
