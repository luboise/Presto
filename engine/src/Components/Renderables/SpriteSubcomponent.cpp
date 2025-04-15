#include "Presto/Objects/Components/Renderables/SpriteComponent.h"

namespace Presto {
SpriteComponent::SpriteComponent(const ImagePtr& asset) {
    this->setAsset(asset);
};

void SpriteComponent::setAsset(const ImagePtr& asset) {
    // TODO: Implement loading the image on the GPU when its needed
    /*
if (!resource.isRegistered()) {
RenderingManager::Get().loadSpriteOnGpu(resource);
}
    */

    asset_ = asset;
}
bool SpriteComponent::hasAsset() const { return asset_ != nullptr; }

ImagePtr SpriteComponent::getAsset() { return asset_; }

}  // namespace Presto
