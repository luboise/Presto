#include "Presto/Assets.h"
#include "Presto/Textures.h"

#include "Modules/RenderingManager.h"

namespace Presto {

Ptr<Texture2D> NewTexture2D(AssetArg path) {
    ImagePtr image{LoadImage(path)};

    if (image == nullptr) {
        PR_ERROR(
            "Unable to load image from path {}. Skipping Texture2D creation.",
            path.string());
        return nullptr;
    }

    return NewTexture2D(image);
};

Ptr<Texture2D> NewTexture2D(const ImagePtr& ptr) {
    return RenderingManager::get().createTexture2D(ptr);
};

}  // namespace Presto
