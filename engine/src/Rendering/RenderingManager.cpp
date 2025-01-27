#include "Presto/Modules/RenderingManager.h"
#include "Presto/Core/Types.h"
#include "Presto/Modules/EntityManager.h"

#include "Presto/Components/Renderable/MeshGroup.h"
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
                                       GLFWAppWindow* window,
                                       Camera& defaultCamera)
        : activeCamera_(defaultCamera) {
        auto* newrenderer_ = Presto::CreateRenderer(library, window);
        renderer_ = newrenderer_;

        // Add default layer
        this->addLayer();
    };

    void RenderingManager::loadMeshOnGpu(ModelAsset& mesh) {
        const std::size_t mesh_count{mesh.meshes_.size()};

        mesh.meshIds_.resize(mesh_count);

        for (std::size_t i = 0; i < mesh_count; i++) {
            const RawMeshData& submesh{mesh.meshes_[i]};

            MeshData mesh_data;

            mesh_data.indices.resize(submesh.indices.size());

            std::ranges::transform(
                submesh.indices.begin(), submesh.indices.end(),
                mesh_data.indices.begin(),
                [](auto val) { return static_cast<Index>(val); });

            mesh_data.vertices.resize(
                std::min({submesh.positions.size(), submesh.normals.size(),
                          submesh.tex_coords.size()}));

            for (std::size_t i = 0; i < mesh_data.vertices.size(); i++) {
                Vertex v = {.position = submesh.positions[i],
                            // Default colour of white
                            .colour = {1, 1, 1},
                            .normal = submesh.normals[i],
                            .tex_coords = submesh.tex_coords[i]};
                mesh_data.vertices[i] = v;
            }

            mesh_data.draw_mode = submesh.draw_mode;
            // data.material = submesh.defaultMaterial_;

            renderer_mesh_id_t new_id{this->renderer_->loadMesh(mesh_data)};

            mesh.meshIds_[i] = new_id;
        }
    };

    void RenderingManager::init(Camera& defaultCamera) {
        PR_CORE_ASSERT(_library != UNSET,
                       "Unable to initialise the RenderingManager with an "
                       "unset graphics library.");
        PR_CORE_ASSERT(_window != nullptr,
                       "Unable to initialise the RenderingManager with a null "
                       "Window handle.");

        instance_ = std::unique_ptr<RenderingManager>(
            new RenderingManager(RenderingManager::_library,
                                 RenderingManager::_window, defaultCamera));
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

        // Update the global uniforms to the current camera
        renderer_->setCameraData(activeCamera_);

        auto& em = EntityManager::get();

        auto mesh_draws{em.findAll() |
                        std::views::transform([](entity_ptr entity) {
                            auto* m_ptr = entity->getComponent<MeshGroup>();
                            auto* t_ptr = entity->getComponent<Transform>();
                            return std::make_tuple(m_ptr, t_ptr);
                        }) |
                        std::views::filter([](auto tuple) {
                            return std::get<0>(tuple) != nullptr &&
                                   std::get<0>(tuple)->hasResource() &&
                                   std::get<1>(tuple) != nullptr;
                        })};

        for (const auto&& [mesh_ptr, transform_ptr] : mesh_draws) {
            mesh_ptr->getMeshResource()->ensureLoaded();
        }

        std::ranges::for_each(
            mesh_draws, [this](std::tuple<MeshGroup*, Transform*> tuple) {
                MeshGroup* m{std::get<0>(tuple)};
                const auto& resource = m->getMesh();

                renderer_->setObjectData(
                    {.transform = std::get<1>(tuple)->getModelView()});

                for (std::size_t i = 0; i < resource.meshIds_.size(); i++) {
                    renderer_->bindMaterial(
                        resource.getSubMeshes()[i].material_data);

                    auto mesh_id{resource.meshIds_[i]};
                    renderer_->render(mesh_id);
                }
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
        activeCamera_ = newCam;
    }

    void RenderingManager::resizeFramebuffer() const {
        renderer_->onFrameBufferResized();
    };

    void RenderingManager::loadImageOnGpu(ImageAsset* resource) {
        if (resource->loaded()) {
            PR_CORE_WARN(
                "Attempted redundant load of image resource {}. Skipping this "
                "load.",
                fmt::ptr(resource));

            return;
        }
        auto image_id{renderer_->loadTexture(resource->getImage())};
        resource->textureId_ = image_id;
    };
}  // namespace Presto
