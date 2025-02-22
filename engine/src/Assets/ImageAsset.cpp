#include "Presto/Assets/ImageAsset.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
bool ImageAsset::load() {
    RenderingManager::get().loadImageOnGpu(*this);
    return true;
}
}  // namespace Presto
