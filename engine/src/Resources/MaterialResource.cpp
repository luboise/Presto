#include "Presto/Resources/MaterialResource.h"

namespace Presto {
    MaterialResource::MaterialResource(PR_STRING_ID name, ImageResource* image)
        : Resource(std::move(name)), image_(image) {}

    void MaterialResource::setImage(ImageResource* image) { image_ = image; }

    Image MaterialResource::getImage() const { return image_->getImage(); }
}  // namespace Presto
