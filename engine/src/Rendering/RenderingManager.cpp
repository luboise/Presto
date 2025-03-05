#include <algorithm>
#include <memory>
#include <ranges>
#include <utility>

#include "Modules/RenderingManager.h"

#include "Presto/Core/Constants.h"
#include "Presto/Objects.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Types/ComponentTypes.h"
#include "Presto/Types/CoreTypes.h"

#include "Presto/Objects/Components.h"

#include "Modules/EntityManagerImpl.h"

#include "Presto/Rendering/Pipeline.h"
#include "Rendering/DefaultMeshes.h"
#include "Rendering/MeshRegistrationData.h"
#include "Rendering/Renderer.h"

// #include "Rendering/Vulkan/VulkanRenderer.h"

#include "Presto/Runtime/GLFWAppWindow.h"

#include "Utils/IDGenerator.h"

#include "Modules/AssetManager.h"

#include "Memory/AllocatorTypes.h"

namespace Presto {

// constexpr auto PR_MIN_USER_PIPELINE_ID = 10;

RENDER_LIBRARY RenderingManager::_library = UNSET;
GLFWAppWindow* RenderingManager::_window = nullptr;

struct RenderingManager::Impl {
    IDGenerator<material_id_t> material_ids;
    IDGenerator<texture_id_t> texture_ids;

    std::vector<MaterialPtr> materials;

    pipeline_allocator_t pipelines{PR_MIN_USER_PIPELINE_ID};
    mesh_allocator_t mesh_registrations{PR_MIN_RUNTIME_MESH_ID};

    std::map<texture_id_t, Ptr<Texture>> textures;

    Allocated<TextureFactory> textureFactory_;
    Allocated<PipelineBuilder> pipelineBuilder_;

    pipeline_id_t current_pipeline_id{PR_PIPELINE_NONE};

    ComponentPtr<CameraComponent> camera_2d;

    struct {
        Ref<MaterialInstance> material;
        Pipeline* pipeline{nullptr};
    } current;
};

RenderingManager::RenderingManager(RENDER_LIBRARY library,
                                   GLFWAppWindow* window,
                                   CameraComponent& defaultCamera)
    : activeCamera_(defaultCamera) {
    this->renderer_ = Renderer::create(library, window);

    impl_ = std::make_unique<Impl>();

    impl_->textureFactory_ = renderer_->getTextureFactory();

    impl_->camera_2d = NewComponent<CameraComponent>();
    impl_->camera_2d->setType(CameraType::ORTHOGRAPHIC)
        .setExtents({.width = 1, .height = 2});

    Renderer::AllocatedPipelineList default_pipelines{
        renderer_->createDefaultPipelines()};

    AssetManager& am{AssetManager::get()};

    for (auto& default_pipeline : default_pipelines) {
        PR_CORE_ASSERT(default_pipeline != nullptr,
                       "The default pipelines must be initialised correctly "
                       "when being used, not nullptr.");
        auto new_definition{
            am.createMaterialDefinition(std::to_string(default_pipeline->id()),
                                        default_pipeline->getStructure())};

        PR_ASSERT(new_definition != nullptr,
                  "The newly created pipeline cannot already exist under a "
                  "different name.");

        auto new_id{default_pipeline->id()};
        impl_->pipelines.alloc(std::move(default_pipeline), new_id);
    }

    // Load the default quad (used for UI rendering)
    loadMesh(DefaultMeshes::Quad, PR_MESH_QUAD);
};

RenderingManager::~RenderingManager() = default;

/*
mesh_registration_id_t RenderingManager::loadMesh(
    MeshData meshData, pipeline_id_t pipelineId,
    mesh_registration_id_t customId) {
    PR_CORE_ASSERT(renderer_ != nullptr,
                   "The renderer must be initialised in order to load meshes.");

    if (pipelineId == PR_PIPELINE_ANY) {
        pipelineId = PR_PIPELINE_DEFAULT_3D;
    }

    Pipeline* pipeline{nullptr};

    pipeline = getPipeline(pipelineId);
    if (pipeline == nullptr) {
        PR_ERROR(
            "Unable to load mesh into pipeline #{}, as it is undefined. "
            "Skipping this mesh load.",
            pipelineId);
        return PR_UNREGISTERED;
    }

    if (impl_->current_pipeline_id != pipelineId) {
        pipeline->bind();
        impl_->current_pipeline_id = pipelineId;
    }

    std::vector<Vertex3D>& vertex_bytes{meshData.vertex_data.vertices};
    Presto::size_t vertex_buffer_size{vertex_bytes.size() * sizeof(Vertex3D)};
    Presto::size_t index_buffer_size{meshData.indices.size() * sizeof(Index)};

    auto details{std::make_unique<MeshRegistrationData>(MeshRegistrationData{
        .render_manager_id{},
        .vertices = renderer_->createBuffer(Buffer::BufferType::VERTEX,
                                            vertex_buffer_size),
        .indices = renderer_->createBuffer(Buffer::BufferType::INDEX,
                                           index_buffer_size),
    })};

    auto& in_vertices{meshData.vertex_data.vertices};

    details->vertices->write(
        std::span<std::byte>(reinterpret_cast<std::byte*>(in_vertices.data()),
                             sizeof(in_vertices[0]) * in_vertices.size()));

    // TODO: Add more write functions here that are optimised for different data
    // types instead of using ErasedBytes
    details->indices->write(std::span<std::byte>(
        reinterpret_cast<std::byte*>(meshData.indices.data()),
        sizeof(meshData.indices[0]) * meshData.indices.size()));

    bool success{
        renderer_->createMeshContext(*details, pipeline->getStructure())};

    if (!success) {
        PR_ERROR("Unable to create mesh context in renderer.");
        return -1;
    }

    if (customId == PR_UNREGISTERED) {
        // Set it to the "define for me" allocation key
        customId = 0;
    }

    auto pair{impl_->mesh_registrations.alloc(std::move(details), customId)};

    mesh_registration_id_t registration_id{pair.first};
    pair.second->render_manager_id = registration_id;

    return registration_id;
};
*/

struct AllocatedStuff {};

template <typename T>
Allocated<MeshRegistrationData> RenderingManager::createMeshRegistration(
    const std::vector<T>& vertices, IndexList& indices,
    pipeline_id_t pipelineId) {
    PR_CORE_ASSERT(renderer_ != nullptr,
                   "The renderer must be initialised in order to load meshes.");

    if (pipelineId == PR_PIPELINE_ANY) {
        pipelineId = PR_PIPELINE_DEFAULT_3D;
    }

    Pipeline* pipeline{nullptr};

    pipeline = getPipeline(pipelineId);
    if (pipeline == nullptr) {
        PR_ERROR(
            "Unable to load mesh into pipeline #{}, as it is undefined. "
            "Skipping this mesh load.",
            pipelineId);
        return nullptr;
    }

    if (impl_->current_pipeline_id != pipelineId) {
        pipeline->bind();
        impl_->current_pipeline_id = pipelineId;
    }

    Presto::size_t vertex_buffer_size{vertices.size() * sizeof(T)};
    Presto::size_t index_buffer_size{indices.size() * sizeof(Index)};

    auto ret{std::make_unique<MeshRegistrationData>(MeshRegistrationData{
        .render_manager_id{},
        .vertices = renderer_->createBuffer(Buffer::BufferType::VERTEX,
                                            vertex_buffer_size),
        .indices = renderer_->createBuffer(Buffer::BufferType::INDEX,
                                           index_buffer_size),
    })};

    ret->vertices->write(
        std::span(reinterpret_cast<const std::byte*>(vertices.data()),
                  vertex_buffer_size));

    // TODO: Add more write functions here that are optimised for different data
    // types instead of using ErasedBytes
    ret->indices->write(std::span(reinterpret_cast<std::byte*>(indices.data()),
                                  index_buffer_size));

    return ret;
};

void RenderingManager::init(CameraComponent& defaultCamera) {
    PR_CORE_ASSERT(_library != UNSET,
                   "Unable to initialise the RenderingManager with an "
                   "unset graphics library.");
    PR_CORE_ASSERT(_window != nullptr,
                   "Unable to initialise the RenderingManager with a null "
                   "Window handle.");

    instance_ = std::unique_ptr<RenderingManager>(new RenderingManager(
        RenderingManager::_library, RenderingManager::_window, defaultCamera));
}

[[nodiscard]] Ptr<Texture2D> RenderingManager::createTexture2D(
    const ImagePtr& image_ptr) {
    auto image{image_ptr->getImage()};

    // TODO: Add width/height validation here
    Ptr<Texture2D> new_texture{createTexture2D(image.width, image.height)};
    new_texture->write(image.bytes);

    return new_texture;
};

Ptr<Texture2D> RenderingManager::createTexture2D(Presto::size_t width,
                                                 Presto::size_t height) {
    texture_id_t new_id{impl_->texture_ids.generate()};
    Ptr<Texture2D> new_texture{impl_->textureFactory_->new2D(width, height)};

    impl_->textures.emplace(new_id,
                            std::static_pointer_cast<Texture>(new_texture));

    return new_texture;
};

void RenderingManager::update() {
    struct DrawStruct {
        ComponentPtr<ModelComponent> model;
        ComponentPtr<TransformComponent> transform;
    };

    // Update the global uniforms to the current camera
    renderer_->setCameraData(activeCamera_);

    auto& em{EntityManagerImpl::get()};

    auto mesh_draws{em.findAll() |
                    std::views::transform([](EntityPtr entity) -> DrawStruct {
                        return {.model = entity->getComponent<ModelComponent>(),
                                .transform =
                                    entity->getComponent<TransformComponent>()};
                    }) |
                    std::views::filter([](const DrawStruct& drawStruct) {
                        return drawStruct.model != nullptr &&
                               drawStruct.model->meshCount() > 0 &&
                               drawStruct.transform != nullptr;
                    })};

    std::ranges::for_each(mesh_draws, [this](const DrawStruct& drawStruct) {
        renderer_->setObjectData(
            {.transform = drawStruct.transform->getModelView()});

        for (std::size_t i = 0; i < drawStruct.model->meshCount(); i++) {
            const MeshPtr& mesh{drawStruct.model->getMesh(i)};

            const MaterialPtr& material{drawStruct.model->getMaterial(i)};

            const auto pipeline_id{material->getPipelineId()};

            // Update pipeline if changed
            if (impl_->current.pipeline == nullptr ||
                impl_->current_pipeline_id != pipeline_id) {
                auto* pipeline{impl_->pipelines.find(pipeline_id)};
                PR_ASSERT(pipeline != nullptr,
                          "Meshes on the draw list can't be drawn to a nullptr "
                          "pipeline.");
                pipeline->bind();
                impl_->current.pipeline = pipeline;
                impl_->current_pipeline_id = pipeline_id;
            }

            // Update material if changed
            if (impl_->current.material.expired() ||
                impl_->current.material.lock() != material) {
                material->bindTo(*impl_->current.pipeline);
                impl_->current.material = material;
            }

            auto* data{impl_->mesh_registrations.find(mesh->registrationId_)};
            PR_CORE_ASSERT(
                data != nullptr,
                "Mesh registrations can't be null at the draw phase.");

            renderer_->render(*data);
        }
    });

    auto canvas_draws{
        em.findAll() |
        std::views::transform(
            [](EntityPtr entity) -> ComponentPtr<CanvasComponent> {
                return entity->getComponent<CanvasComponent>();
            }) |
        std::views::filter(
            [](const auto& canvas) { return canvas != nullptr; })};

    auto* ui_pipeline{getPipeline(PR_PIPELINE_DEFAULT_UI)};

    ui_pipeline->bind();

    auto* quad_registration{impl_->mesh_registrations.find(PR_MESH_QUAD)};
    PR_CORE_ASSERT(quad_registration != nullptr,
                   "The default quad can not be null.");

    renderer_->setCameraData(*impl_->camera_2d);

    // Set pipeline to UI pipeline
    for (const ComponentPtr<CanvasComponent>& ptr : canvas_draws) {
        for (const CanvasGroup& group : ptr->groups_) {
            // Render each canvasitem where it should be
            for (const CanvasItem& canvasItem : group.items()) {
                if (canvasItem.texture() == nullptr) {
                    PR_ERROR("CanvasItem has no texture. Skipping this draw.");
                    continue;
                }

                canvasItem.texture()->bind(2);
                renderer_->render(*quad_registration);
            }
        }
    }

    // Unbind all bound resources
    impl_->current_pipeline_id = PR_PIPELINE_NONE;
    impl_->current.pipeline = nullptr;

    impl_->current.material.reset();

    // TODO: Refactor this to cache in the RenderingManager if the
    // performance impact is too much
}

void RenderingManager::clear() { renderer_->nextFrame(); }

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

const PipelineStructure* RenderingManager::getPipelineStructure(
    pipeline_id_t id) const {
    auto transform_view{
        impl_->pipelines | std::views::values |
        std::views::transform(
            [](const Allocated<Pipeline>& val) -> const PipelineStructure& {
                return val->getStructure();
            })};

    for (const auto& view : transform_view) {
        if (view.pipeline_id == id) {
            return &view;
        }
    }

    return nullptr;
};

Allocated<UniformBuffer> RenderingManager::createUniformBuffer(
    Presto::size_t size) {
    return renderer_->createUniformBuffer(size);
};

Ptr<MaterialInstance> RenderingManager::createMaterial(MaterialType type,
                                                       Presto::string name) {
    // TODO: Make this a constexpr
    pipeline_id_t id{[type]() {
        switch (type) {
            case MaterialType::DEFAULT_3D:
                return PR_PIPELINE_DEFAULT_3D;
            case MaterialType::DEFAULT_UI:
                return PR_PIPELINE_DEFAULT_UI;
        }
    }()};

    auto definition{AssetManager::get().getMaterialDefinition(id)};
    PR_CORE_ASSERT(
        definition != nullptr,
        "A default pipeline can't have a null shared pointer to it.");

    auto new_instance{std::make_shared<MaterialInstance>(definition)};
    new_instance->setName(std::move(name));

    impl_->materials.push_back(new_instance);

    return new_instance;
};

Ptr<MaterialInstance> RenderingManager::findMaterial(
    const Presto::string& name) {
    if (auto found{std::ranges::find_if(
            impl_->materials,
            [name](const MaterialPtr& val) { return val->name() == name; })};
        found != impl_->materials.end()) {
        return *found;
    }

    return nullptr;
};

PipelineBuilder& RenderingManager::getPipelineBuilder() {
    PR_CORE_ASSERT(impl_->pipelineBuilder_ != nullptr,
                   "The application must be initialised in order to "
                   "get the pipeline "
                   "builder.");

    return *impl_->pipelineBuilder_;
};

Pipeline* RenderingManager::getPipeline(pipeline_id_t id) const {
    auto* pipeline{impl_->pipelines.find(id)};
    if (pipeline == nullptr) {
        PR_WARN(
            "Pipeline of id {} could not be found in the RenderingManager. ",
            id);
    }

    return pipeline;
};

mesh_registration_id_t RenderingManager::loadMesh(
    MeshData meshData, mesh_registration_id_t customId) {
    PR_CORE_ASSERT(renderer_ != nullptr,
                   "The renderer must be initialised in order to load meshes.");

    auto pipelineId{meshData.pipeline_id};

    if (pipelineId == PR_PIPELINE_ANY) {
        pipelineId = PR_PIPELINE_DEFAULT_3D;
    }

    Pipeline* pipeline{nullptr};

    pipeline = getPipeline(pipelineId);
    if (pipeline == nullptr) {
        PR_ERROR(
            "Unable to load mesh into pipeline #{}, as it is undefined. "
            "Skipping this mesh load.",
            pipelineId);
        return PR_UNREGISTERED;
    }

    if (impl_->current_pipeline_id != pipelineId) {
        pipeline->bind();
        impl_->current_pipeline_id = pipelineId;
    }

    Allocated<MeshRegistrationData> details;
    std::visit(
        [&]<typename T>(const std::vector<T>& vertices) {
            details = createMeshRegistration(vertices, meshData.indices,
                                             meshData.pipeline_id);
        },
        meshData.vertices);

    if (details == nullptr) {
        PR_ERROR("Unable to create mesh registration from MeshData.");
        return PR_UNREGISTERED;
    }

    bool success{
        renderer_->createMeshContext(*details, pipeline->getStructure())};

    if (!success) {
        PR_ERROR("Unable to create mesh context in renderer.");
        return PR_UNREGISTERED;
    }

    if (customId == PR_UNREGISTERED) {
        // Set it to the "define for me" allocation key
        customId = 0;
    }

    auto pair{impl_->mesh_registrations.alloc(std::move(details), customId)};

    mesh_registration_id_t registration_id{pair.first};
    pair.second->render_manager_id = registration_id;

    return registration_id;
};
}  // namespace Presto
