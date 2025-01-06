#include "Presto/Modules/RenderingManager.h"
#include "Presto/Modules/EntityManager.h"

#include "Presto/Components/Renderable/Mesh.h"
// #include "Presto/Components/RenderableProps.h"
#include "Presto/Components/Transform.h"
#include "Presto/Rendering/Renderer.h"

// #include "Rendering/Vulkan/VulkanRenderer.h"

#include "Presto/Rendering/RendererFactory.h"
#include "Presto/Runtime/GLFWAppWindow.h"

#include <algorithm>

namespace Presto {

    RENDER_LIBRARY RenderingManager::_library = UNSET;
    GLFWAppWindow* RenderingManager::_window = nullptr;

    RenderingManager::RenderingManager(RENDER_LIBRARY library,
                                       GLFWAppWindow* window) {
        auto* newrenderer_ = Presto::CreateRenderer(library, window);
        renderer_ = newrenderer_;

        // Add default layer
        this->addLayer();
    };

    void RenderingManager::loadMeshOnGpu(MeshResource& mesh) {
        RenderData data{};
        data.indices = mesh.indices;
        data.vertices = mesh.vertices;

        render_data_id_t r_id = renderer_->registerMesh(data);

        mesh.renderId_ = r_id;
    };

    void RenderingManager::Init() {
        PR_CORE_ASSERT(_library != UNSET,
                       "Unable to initialise the RenderingManager with an "
                       "unset graphics library.");
        PR_CORE_ASSERT(_window != nullptr,
                       "Unable to initialise the RenderingManager with a null "
                       "Window handle.");

        instance_ = std::unique_ptr<RenderingManager>(new RenderingManager(
            RenderingManager::_library, RenderingManager::_window));
    }

    void RenderingManager::Update() {
        /*
for (auto& layer : _renderLayers) {
    for (const auto& ptr_renderable : layer._renderables) {
        renderer_->render(ptr_renderable,
                          currentCamera_->getViewMatrix());
    }
}
renderer_->nextFrame();
        */
        auto& em = EntityManager::Get();

        auto mesh_draws =
            em.findAll() | std::views::values |
            std::views::transform([](entity_ptr entity) {
                return std::make_tuple(entity->getComponent<Mesh>(),
                                       entity->getComponent<Transform>());
            }) |
            std::views::filter([](auto tuple) {
                return std::get<0>(tuple) != nullptr &&
                       std::get<1>(tuple) != nullptr;
            });

        std::ranges::for_each(
            mesh_draws, [this](std::tuple<Mesh*, Transform*> tuple) {
                renderer_->render(std::get<0>(tuple)->getResource().renderId_,
                                  std::get<1>(tuple)->getModelView());
            });

        // TODO: Refactor this to cache in the RenderingManager if the
        // performance impact is too much
    }

    void RenderingManager::Shutdown() {}

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

    /*
        Mesh* RenderingManager::NewMesh(const VertexList& vertices,
                                        const IndexList& indices) {
            auto* mesh{new Mesh(_meshes.getNextId(), vertices, indices)};
            _meshes.add(mesh);

            return mesh;
        }
    */

    /*
        void RenderingManager::AddRenderable(layer_id_t layer_index,
                                             Renderable* ptr_renderable) {
            if (_renderables.isAllocated(ptr_renderable)) {
                RenderingManager::RemoveRenderable(ptr_renderable);
            }

            RenderLayer& layer = getLayer(layer_index);
            renderer_->addToRenderPool(ptr_renderable);
            layer.addRenderable(ptr_renderable);
        }

        RenderableProps* RenderingManager::NewRenderableProps() {
            auto* props{new RenderableProps()};
            _renderProps.add(props);

            return props;
        }

        Renderable* RenderingManager::NewRenderable(
            PrestoRenderableConstructorArgs) {
            auto* renderable{new Renderable(mesh, props)};
            // _renderables[];
            return renderable;
        }
            */
}  // namespace Presto
