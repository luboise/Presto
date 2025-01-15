#include "Presto/Components/Renderable/Sprite.h"

#include "Presto/Rendering/RenderData.h"

namespace Presto {
    RenderData Sprite::getRenderData() const {
        // TODO: Implement this
        return {};
    };

    void Sprite::setResource(ImageResource& resource) {
        // TODO: Implement loading the image on the GPU when its needed
        /*
if (!resource.isRegistered()) {
    RenderingManager::Get().loadSpriteOnGpu(resource);
}
        */

        resource_ = &resource;
    }
    bool Sprite::hasResource() const { return resource_ != nullptr; }

    Sprite::Sprite(ImageResource& resource) { this->setResource(resource); };
}  // namespace Presto
