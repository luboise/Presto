#include "Presto/Resources/MaterialResource.h"

namespace Presto {
    MaterialResource::MaterialResource(PR_STRING_ID name, ImageResource* image)
        : Resource(std::move(name)), albedoImage_(image) {}

    void MaterialResource::setImage(ImageResource* image) {
        albedoImage_ = image;
    }

    Image MaterialResource::getImage() const {
        return albedoImage_->getImage();
    }
    MaterialData MaterialResource::getData() const {
        return {image_->getImage()};
    }
}  // namespace Presto
