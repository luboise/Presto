#include "Presto/Resources/ImageResource.h"

#include "Presto/Modules/RenderingManager.h"

namespace Presto {
    void ImageResource::load() { RenderingManager::get().loadImageOnGpu(this); }
}  // namespace Presto
