#include "Presto/Resources/MaterialResource.h"
#include "Presto/Core/Constants.h"
#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    MaterialAsset::MaterialAsset(PR_STRING_ID name, ImageAsset* image)
        : Asset(std::move(name)), diffuseImage_(image) {}

    void MaterialAsset::setImage(ImageAsset* image) {
        diffuseImage_ = image;
        diffuseImage_->ensureLoaded();
    }

    Image MaterialAsset::getImage() const { return diffuseImage_->getImage(); }

    MaterialData MaterialAsset::getData() const {
        MaterialData data{};
        data.materialType = PR_MATERIAL_DEFAULT_3D;
        if (diffuseImage_ != nullptr) {
            data.diffuseTexture = diffuseImage_->getRenderId();
        }

        return data;
    }

    void MaterialAsset::load() {
        RenderingManager::get().loadImageOnGpu(this->diffuseImage_);
    };
}  // namespace Presto
