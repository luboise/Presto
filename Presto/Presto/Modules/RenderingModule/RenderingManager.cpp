#include "RenderingManager.h"

#include "Vulkan/VulkanRenderer.h"

#include <set>

namespace Presto {

    void RenderingManager::F_INIT(Renderer::RENDER_LIBRARY library,
                                  GLFWwindow* windowPtr) {
        _window = windowPtr;

        if (_renderer != nullptr) {
            delete _renderer;
            _renderer = nullptr;
        }

        // TODO: Add logic to choose renderer
        _renderer = new VulkanRenderer(windowPtr);

        addLayer();
        _initialised = true;
    }

    void RenderingManager::F_UPDATE() {
        for (auto& layer : _renderLayers) {
            for (auto& entity : layer._entities) {
                _renderer->draw(entity);
            }
        }
        _renderer->nextFrame();
    }
    void RenderingManager::F_SHUTDOWN() {}

    layer_id_t RenderingManager::addLayer(size_t pos) {
        if (pos == (size_t)-1) {
            pos = _renderLayers.size();
        } else {
            PR_ASSERT(pos < _renderLayers.size(),
                      "Invalid position given for new layer \"{}\".", pos);
        }

        _renderLayers.insert(_renderLayers.begin() + pos, RenderLayer());
        return pos;
    }

    void RenderingManager::removeLayer(layer_id_t id) {
        PR_CORE_ASSERT(hasLayer(id),
                       "Attempted to remove layer with invalid index {}."
                       "\tMax allowed index: {}",
                       id, _renderLayers.size() - 1);
        _renderLayers.erase(_renderLayers.begin() + id);
    }

    void RenderingManager::addEntity(layer_id_t layer_index,
                                     Entity* ptr_entity) {
        if (_boundEntities.find(ptr_entity) != _boundEntities.end()) {
            removeEntity(ptr_entity);
        }

        RenderLayer& layer = getLayer(layer_index);
        layer.addEntity(ptr_entity);
    }

    // TODO: Implement remove entity
    void RenderingManager::removeEntity(Entity* ptr_entity) {}

    bool RenderingManager::hasLayer(layer_id_t index) {
        return (index < _renderLayers.size());
    }

    RenderLayer& RenderingManager::getLayer(layer_id_t id) {
        PR_CORE_ASSERT(_renderLayers.size() > 0,
                       "Attempted to get layer {} but there are no layers.",
                       id);

        PR_CORE_ASSERT(
            hasLayer(id),
            "Attempted to get layer with invalid index {}. Expected [0,{}]", id,
            _renderLayers.size() - 1);

        return _renderLayers[id];
    }
}  // namespace Presto
