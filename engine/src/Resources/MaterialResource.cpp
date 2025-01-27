#include "Presto/Resources/MaterialResource.h"
#include "Presto/Core/Constants.h"
#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    MaterialResource::MaterialResource(PR_STRING_ID name, ImageResource* image)
        : Resource(std::move(name)), diffuseImage_(image) {}

    void MaterialResource::setImage(ImageResource* image) {
        diffuseImage_ = image;
        diffuseImage_->ensureLoaded();
    }

    Image MaterialResource::getImage() const {
        return diffuseImage_->getImage();
    }

    MaterialData MaterialResource::getData() const {
        MaterialData data{};
        data.materialType = PR_MATERIAL_DEFAULT_3D;
        if (diffuseImage_ != nullptr) {
            data.diffuseTexture = diffuseImage_->getRenderId();
        }

        return data;
    }

    void MaterialResource::load() {
        RenderingManager::get().loadImageOnGpu(this->diffuseImage_);
    };
}  // namespace Presto
