#include "Presto/Components/Renderable/Sprite.h"

namespace Presto {
    void Sprite::setResource(ImageAsset& resource) {
        // TODO: Implement loading the image on the GPU when its needed
        /*
if (!resource.isRegistered()) {
    RenderingManager::Get().loadSpriteOnGpu(resource);
}
        */

        resource_ = &resource;
    }
    bool Sprite::hasResource() const { return resource_ != nullptr; }

    Sprite::Sprite(ImageAsset& resource) { this->setResource(resource); };
}  // namespace Presto
