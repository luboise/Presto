#include "Presto/Rendering/RenderingManager.h"

#include "Presto/Components/Renderable.h"
#include "Presto/Components/RenderableProps.h"
#include "Presto/Rendering/Mesh.h"
#include "Presto/Rendering/Vertex.h"
#include "Rendering/Vulkan/VulkanRenderer.h"

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

        /**
_meshes = Allocator<Mesh>();
_renderProps = std::map<id_t, RenderableProps*>();
_renderables = std::map<id_t, Renderable*>();
        **/

        AddLayer();
        _initialised = true;
    }

    void RenderingManager::F_UPDATE() {
        for (auto& layer : _renderLayers) {
            for (auto& ptr_renderable : layer._renderables) {
                _renderer->draw(ptr_renderable);
            }
        }
        _renderer->nextFrame();
    }
    void RenderingManager::F_SHUTDOWN() {}

    layer_id_t RenderingManager::AddLayer(size_t pos) {
        if (pos == (size_t)-1) {
            pos = _renderLayers.size();
        } else {
            PR_ASSERT(pos < _renderLayers.size(),
                      "Invalid position given for new layer \"{}\".", pos);
        }

        _renderLayers.insert(_renderLayers.begin() + pos, RenderLayer());
        return pos;
    }

    void RenderingManager::RemoveLayer(layer_id_t id) {
        PR_CORE_ASSERT(hasLayer(id),
                       "Attempted to remove layer with invalid index {}."
                       "\tMax allowed index: {}",
                       id, _renderLayers.size() - 1);
        _renderLayers.erase(_renderLayers.begin() + id);
    }

    void RenderingManager::AddRenderable(layer_id_t layer_index,
                                         Renderable* ptr_renderable) {
        if (_renderables.IsAllocated(ptr_renderable))
            RenderingManager::RemoveRenderable(ptr_renderable);

        RenderLayer& layer = getLayer(layer_index);
        _renderer->AddToRenderPool(ptr_renderable);
        layer.addRenderable(ptr_renderable);
    }

    // TODO: Implement remove entity
    void RenderingManager::RemoveRenderable(Renderable* ptr_renderable) {
        _renderables.Release(ptr_renderable);
    }

    void RenderingManager::SetCamera(Camera& newCam) {
        _renderer->setCamera(newCam);
    }

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

    Mesh* RenderingManager::NewMesh(const VertexList& vertices,
                                    const IndexList& indices) {
        auto* mesh = new Mesh(_meshes.GetNextId(), vertices, indices);
        _meshes.Add(mesh);

        return mesh;
    }

    RenderableProps* RenderingManager::NewRenderableProps() {
        auto* props = new RenderableProps();
        _renderProps.Add(props);

        return props;
    }

    Renderable* RenderingManager::NewRenderable(
        PrestoRenderableConstructorArgs) {
        auto renderable = new Renderable(mesh, props);
        // _renderables[];
        return renderable;
    }
}  // namespace Presto
