#include <utility>

#include "Presto/Assets/MaterialAsset.h"

#include "Presto/Core/Constants.h"
#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    MaterialAsset::MaterialAsset(PR_STRING_ID name, ImagePtr image)
        : Asset(std::move(name)), diffuseImage_(std::move(image)) {}

    void MaterialAsset::setDiffuseTexture(ImagePtr image) {
        diffuseImage_ = std::move(image);
        diffuseImage_->ensureLoaded();
    }

    Image MaterialAsset::getImage() const { return diffuseImage_->getImage(); }

    MaterialData MaterialAsset::getData() const {
        MaterialData data{};
        data.materialType = PR_PIPELINE_DEFAULT_3D;
        if (diffuseImage_ != nullptr) {
            data.diffuseTexture = diffuseImage_->getRenderId();
        }

        return data;
    }

    void MaterialAsset::load() {
        RenderingManager::get().loadImageOnGpu(this->diffuseImage_);
    };
}  // namespace Presto
