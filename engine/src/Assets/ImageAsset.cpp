#include "Presto/Assets/ImageAsset.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    void ImageAsset::load() { RenderingManager::get().loadImageOnGpu(*this); }
}  // namespace Presto
