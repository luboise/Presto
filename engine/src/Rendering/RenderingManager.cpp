#include "Presto/Modules/RenderingManager.h"
#include "Presto/Modules/EntityManager.h"

#include "Presto/Components/Renderable/ModelComponent.h"
#include "Presto/Components/TransformComponent.h"
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
                                       CameraComponent& defaultCamera)
        : activeCamera_(defaultCamera) {
        auto* newrenderer_ = Presto::CreateRenderer(library, window);
        renderer_ = newrenderer_;

        // Add default layer
        // this->addLayer();
    };

    void RenderingManager::loadMeshOnGpu(MeshAsset& mesh) {
        auto new_id{renderer_->loadMesh(mesh.data_)};
        mesh.renderId_ = new_id;
    };

    void RenderingManager::loadModelOnGpu(ModelAsset& model) {
        for (MeshPtr& mesh : model.meshes_) {
            mesh->ensureLoaded();
        }
    }

    void RenderingManager::init(CameraComponent& defaultCamera) {
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

        struct DrawStruct {
            ComponentPtr<ModelComponent> model;
            ComponentPtr<TransformComponent> transform;
        };

        // Update the global uniforms to the current camera
        renderer_->setCameraData(activeCamera_);

        auto& em = EntityManager::get();

        auto mesh_draws{
            em.findAll() |
            std::views::transform([](entity_ptr entity) -> DrawStruct {
                return {
                    .model = entity->getComponent<ModelComponent>(),
                    .transform = entity->getComponent<TransformComponent>()};
            }) |
            std::views::filter([](const DrawStruct& drawStruct) {
                return drawStruct.model != nullptr &&
                       drawStruct.model->meshCount() > 0 &&
                       drawStruct.transform != nullptr;
            })};

        /*
                for (const auto&& [model_ptr, transform_ptr] : mesh_draws) {
                    for (size_t i = 0; i < model_ptr->size(); i++) {
                        model_ptr->getModelAsset()->ensureLoaded();
                    }
                }
                        */

        std::ranges::for_each(mesh_draws, [this](const DrawStruct& drawStruct) {
            renderer_->setObjectData(
                {.transform = drawStruct.transform->getModelView()});

            for (std::size_t i = 0; i < drawStruct.model->meshCount(); i++) {
                const auto& mesh{drawStruct.model->getMesh(i)};
                const auto& material{drawStruct.model->getMaterial(i)};

                MaterialData mat_data{};

                if (material != nullptr) {
                    mat_data = material->getData();
                } else if (!mesh->defaultMaterial_.expired()) {
                    mat_data = mesh->defaultMaterial_.lock()->getData();
                }

                renderer_->bindMaterial(mat_data);

                auto mesh_id{mesh->renderId_};
                renderer_->render(mesh_id);
            }
        });

        // TODO: Refactor this to cache in the RenderingManager if the
        // performance impact is too much
    }

    void RenderingManager::shutdown() {}

    /*
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
    */

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

    void RenderingManager::setCamera(CameraComponent& newCam) {
        PR_CORE_ASSERT(RenderingManager::initialised(),
                       "Unable to set camera when the RenderingManager is "
                       "uninitialised.")
        activeCamera_ = newCam;
    }

    void RenderingManager::resizeFramebuffer() const {
        renderer_->onFrameBufferResized();
    };

    void RenderingManager::loadImageOnGpu(const ImagePtr& image) {
        image->ensureLoaded();
    };

    void RenderingManager::loadImageOnGpu(ImageAsset& image) {
        if (image.loaded()) {
            PR_CORE_WARN(
                "Attempted redundant load of image resource {}. Skipping this "
                "load.",
                image.renderId_);

            return;
        }
        auto image_id{renderer_->loadTexture(image.getImage())};
        image.renderId_ = image_id;
    };
}  // namespace Presto
