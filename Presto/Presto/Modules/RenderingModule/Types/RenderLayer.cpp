#include "RenderLayer.h"

namespace Presto {
    RenderLayer::RenderLayer() {}
    RenderLayer::~RenderLayer() {}

    void RenderLayer::addEntity(Entity* entity) { _entities.push_back(entity); }

}  // namespace Presto
