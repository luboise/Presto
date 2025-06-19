export module presto.objects.components.canvas:canvasitems;

import presto.core;

export namespace Presto {

class ImageAsset;

class PRESTO_API CanvasButton : public CanvasItem {
   public:
    explicit CanvasButton(CanvasPosition position);

    void setImage(const ImagePtr&);

   private:
    ImagePtr image_;
};

}  // namespace Presto
