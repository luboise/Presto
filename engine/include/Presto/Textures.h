#include "Presto/Rendering/TextureTypes.h"
#include "Presto/Types/AssetTypes.h"

namespace Presto {
/**
 * @brief  Creates a new texture based on a filepath. This is the equivalent to
 * loading the image first to obtain an ImagePtr, then calling
 * loadTexture2D(ptr)
 */
Ptr<Texture2D> NewTexture2D(AssetArg path);
Ptr<Texture2D> NewTexture2D(const ImagePtr& ptr);

}  // namespace Presto
