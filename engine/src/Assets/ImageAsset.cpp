#include "Presto/Assets/ImageAsset.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    void ImageAsset::load() {
        RenderingManager::get().loadImageOnGpu(
            std::shared_ptr<ImageAsset>(this));
    }
}  // namespace Presto
