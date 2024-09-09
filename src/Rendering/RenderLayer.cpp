#include "Presto/Rendering/types/RenderLayer.h"

namespace Presto {
    RenderLayer::RenderLayer() {}
    RenderLayer::~RenderLayer() {}

    void RenderLayer::addRenderable(Renderable* renderable) {
        _renderables.push_back(renderable);
    }

}  // namespace Presto