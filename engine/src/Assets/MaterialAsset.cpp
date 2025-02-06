#include <utility>

#include "Presto/Assets/MaterialAsset.h"

#include "Presto/Assets/ImportTypes.h"
#include "Presto/Core/Constants.h"
#include "Presto/Modules/RenderingManager.h"

namespace Presto {
MaterialAsset::MaterialAsset(PR_STRING_ID name,
                             const ImportedMaterial& importedMaterial)
    : Asset(std::move(name)) {}

void MaterialAsset::setDiffuseTexture(ImagePtr image) {
    diffuseImage_ = std::move(image);
    diffuseImage_->ensureLoaded();
}

ImagePtr MaterialAsset::getImage() const { return diffuseImage_; }

MaterialStructure MaterialAsset::getStructure() const {
    MaterialStructure data{};
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
