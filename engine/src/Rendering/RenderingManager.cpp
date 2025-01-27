#include "Presto/Modules/RenderingManager.h"
#include "Presto/Core/Types.h"
#include "Presto/Modules/EntityManager.h"

#include "Presto/Components/Renderable/Mesh.h"
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
        RenderGroup group;

        for (const auto& submesh : mesh.sub_meshes) {
            RenderData data;

            data.indices.resize(submesh.indices.size());

            std::ranges::transform(
                submesh.indices.begin(), submesh.indices.end(),
                data.indices.begin(),
                [](auto val) { return static_cast<Index>(val); });

            data.vertices.resize(
                std::min({submesh.positions.size(), submesh.normals.size(),
                          submesh.tex_coords.size()}));

            for (int i = 0; i < data.vertices.size(); i++) {
                Vertex v = {.position = submesh.positions[i],
                            // Default colour of white
                            .colour = {1, 1, 1},
                            .normal = submesh.normals[i],
                            .tex_coords = submesh.tex_coords[i]};
                data.vertices[i] = v;
            }

            data.draw_mode = submesh.draw_mode;
            data.material = submesh.material;

            group.render_list.push_back(data);
        }

        render_data_id_t r_id = renderer_->registerRenderGroup(group);
        mesh.renderId_ = r_id;
    };

    void RenderingManager::init() {
        PR_CORE_ASSERT(_library != UNSET,
                       "Unable to initialise the RenderingManager with an "
                       "unset graphics library.");
        PR_CORE_ASSERT(_window != nullptr,
                       "Unable to initialise the RenderingManager with a null "
                       "Window handle.");

        instance_ = std::unique_ptr<RenderingManager>(new RenderingManager(
            RenderingManager::_library, RenderingManager::_window));
    }

    void RenderingManager::update() {
        /*
for (auto& layer : _renderLayers) {
    for (const auto& ptr_renderable : layer._renderables) {
        renderer_->render(ptr_renderable,
                          currentCamera_->getViewMatrix());
    }
}
        */

        // Update the camera
        renderer_->setViewMatrix(camera_.getViewMatrix());

        auto& em = EntityManager::Get();

        auto mesh_draws = em.findAll() |
                          std::views::transform([](entity_ptr entity) {
                              auto* m_ptr = entity->getComponent<Mesh>();
                              auto* t_ptr = entity->getComponent<Transform>();
                              return std::make_tuple(m_ptr, t_ptr);
                          }) |
                          std::views::filter([](auto tuple) {
                              return std::get<0>(tuple) != nullptr &&
                                     std::get<0>(tuple)->hasResource() &&
                                     std::get<1>(tuple) != nullptr;
                          });

        std::ranges::for_each(
            mesh_draws, [this](std::tuple<Mesh*, Transform*> tuple) {
                Mesh* m{std::get<0>(tuple)};
                const auto& resource = m->getResource();

                renderer_->render(resource.renderId_,
                                  std::get<1>(tuple)->getModelView());
            });

        // TODO: Refactor this to cache in the RenderingManager if the
        // performance impact is too much
    }

    void RenderingManager::shutdown() {}

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

    void RenderingManager::clear() { renderer_->nextFrame(); }

    /*
        Mesh* RenderingManager::NewMesh(const VertexList& vertices,
                                        const IndexList& indices) {
            auto* mesh{new Mesh(_meshes.getNextId(), vertices, indices)};
            _meshes.add(mesh);

            return mesh;
        }
    */
    void RenderingManager::setRenderLibrary(RENDER_LIBRARY library) {
        PR_CORE_ASSERT(!RenderingManager::initialised(),
                       "Unable to set render library while the renderer is "
                       "already initialised.");

        RenderingManager::_library = library;
    }
    void RenderingManager::setWindow(GLFWAppWindow* window) {
        PR_CORE_ASSERT(!RenderingManager::initialised(),
                       "Unable to set window surface while the renderer is "
                       "already initialised.");

        RenderingManager::_window = window;
    }

    void RenderingManager::setCamera(Camera& newCam) {
        PR_CORE_ASSERT(RenderingManager::initialised(),
                       "Unable to set camera when the RenderingManager is "
                       "uninitialised.")
        camera_ = newCam;
    }
}  // namespace Presto
