#include "Presto/Rendering/RenderingManager.h"

#include <stdexcept>

#include "Presto/Components/Renderable.h"
#include "Presto/Components/Transform.h"

#include "Presto/Rendering/Mesh.h"
#include "Presto/Rendering/Renderer.h"
#include "Presto/Rendering/Vertex.h"

#include "Presto/Rendering/Types/RenderLayer.h"
#include "Rendering/OpenGL/OpenGLRenderer.h"
#include "Rendering/Vulkan/VulkanRenderer.h"

#include "GLFWAppWindow.h"

namespace Presto {

    RENDER_LIBRARY RenderingManager::_library = UNSET;
    GLFWAppWindow* RenderingManager::_window = nullptr;

    RenderingManager::RenderingManager(RENDER_LIBRARY library,
                                       GLFWAppWindow* window) {
        switch (library) {
            case OPENGL:
                _renderer = new OpenGLRenderer(window);
                break;
            case VULKAN:
                _renderer = new VulkanRenderer(window);
                break;
            default:
                throw std::runtime_error("Invalid render library specified.");
        }

        // Add default layer
        this->AddLayer();
    };

    void RenderingManager::Init() {
        PR_CORE_ASSERT(_library != UNSET,
                       "Unable to initialise the RenderingManager with an "
                       "unset graphics library.");
        PR_CORE_ASSERT(_window != nullptr,
                       "Unable to initialise the RenderingManager with a null "
                       "Window handle.");

        RenderingManager* mgr = new RenderingManager(RenderingManager::_library,
                                                     RenderingManager::_window);
        _instance = mgr;
    }

    // Draws all entities
    void RenderingManager::Update() {
        for (auto& layer : _renderLayers) {
            for (const auto& ptr_renderable : layer._renderables) {
                _renderer->render(ptr_renderable);
            }
        }
    }

    void RenderingManager::Flush() { _renderer->nextFrame(); }

    void RenderingManager::Shutdown() {}

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
        if (_renderables.isAllocated(ptr_renderable))
            RenderingManager::RemoveRenderable(ptr_renderable);

        RenderLayer& layer = getLayer(layer_index);
        _renderer->addToRenderPool(ptr_renderable);
        layer.addRenderable(ptr_renderable);
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
        auto* mesh = new Mesh(_meshes.getNextId(), vertices, indices);
        _meshes.add(mesh);

        return mesh;
    }

    Renderable* RenderingManager::NewRenderable(
        PrestoRenderableConstructorArgs) {
        auto renderable = new Renderable(mesh, props);
        // _renderables[];
        return renderable;
    }
}  // namespace Presto
