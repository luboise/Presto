#include "Presto/Rendering/RenderLayer.h"
#include "Presto/Objects/Components/Renderable.h"

namespace Presto {
RenderLayer::RenderLayer() {}
RenderLayer::~RenderLayer() {}

void RenderLayer::addRenderable(Renderable* renderable) {
    _renderables.push_back(renderable);
}

}  // namespace Presto
