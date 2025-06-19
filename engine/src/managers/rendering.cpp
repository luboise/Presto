module presto.internal.managers.rendering;

import presto.internal;

// STL import
#include <algorithm>
#include <memory>
#include <ranges>
#include <utility>

// Public imports
#include "Modules/EventManagerImpl.h"
#include "Presto/Assets/ImportTypes.h"
#include "Presto/Core/Constants.h"
#include "Presto/Objects.h"
#include "Presto/Objects/Components.h"
#include "Presto/Platform.h"
#include "Presto/Rendering/MeshData.h"
#include "Presto/Rendering/Pipeline.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Runtime/Events/ApplicationEvents.h"
#include "Presto/Runtime/GLFWAppWindow.h"
#include "Presto/Types/CoreTypes.h"
#include "Presto/Types/ObjectTypes.h"

// Internal imports
#include "Memory/AllocatorTypes.h"

#include "Rendering/MeshRegistrationData.h"
#include "Rendering/Renderer.h"
#include "Utils/IDGenerator.h"

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
    std::vector<Mesh> meshes;

    std::map<texture_id_t, Ptr<Texture>> textures;

    Allocated<TextureFactory> texture_factory;
    Allocated<PipelineBuilder> pipeline_builder;

    pipeline_id_t current_pipeline_id{PR_PIPELINE_NONE};

    bool using_debug_cam{false};

    EntityPtr cam_active_entity;

    ComponentPtr<CameraComponent> cam_debug;
    ComponentPtr<CameraComponent> cam_2d;

    struct {
        Ref<MaterialInstance> material;
        AllocatedPipeline* pipeline{nullptr};
    } current;
};

RenderingManager::RenderingManager(RENDER_LIBRARY library,
                                   GLFWAppWindow* window) {
    this->renderer_ = Renderer::create(library, window);

    impl_ = std::make_unique<Impl>();

    impl_->texture_factory = renderer_->getTextureFactory();

    auto active_camera{NewComponent<CameraComponent>()};

    impl_->cam_active_entity = EntityManagerImpl::get().newEntity();
    impl_->cam_active_entity->setComponent(active_camera);

    active_camera->setFOV(DEFAULT_FOV);

    auto cam_2d{NewComponent<CameraComponent>()};

    impl_->cam_2d = NewComponent<CameraComponent>();
    impl_->cam_2d->setType(CameraType::ORTHOGRAPHIC)
        .setExtents({.width = 1, .height = 2});

    impl_->cam_debug = NewComponent<CameraComponent>();
    impl_->cam_debug->setFOV(DEFAULT_FOV);

    auto& evm{EventManagerImpl::get()};

    evm.addHandler<FramebufferResizedEvent>(
        [this](FramebufferResizedEvent& event) {
            renderer_->setExtents(event.extents());
        });
};

void RenderingManager::loadDefaults() {
    if (!impl_->pipelines.empty()) {
        PR_WARN(
            "Attempted to initialise the default pipelines when they are "
            "already set up. Ignoring this request.");
        return;
    }

    AssetManager& am{AssetManager::get()};

    Renderer::AllocatedPipelineList default_pipelines{
        renderer_->createDefaultPipelines()};

    TexturePtr fallback_tex{
        this->createTexture2D(DEFAULT_TEXTURE_DATA, PR_TEX_DIFFUSE_FALLBACK)};
    TexturePtr flat_shading_tex{
        this->createTexture2D(DEFAULT_TEXTURE_DATA, PR_TEX_DIFFUSE_FLAT)};

    // Check that all of them loaded correctly
    std::ranges::for_each(
        default_pipelines, [](const Allocated<Pipeline>& default_pipeline) {
            PR_CORE_ASSERT(
                default_pipeline != nullptr,
                "The default pipelines must be initialised correctly "
                "when being used, not nullptr.");
        });

    for (Allocated<Pipeline>& default_pipeline : default_pipelines) {
        auto new_definition{
            am.createMaterialDefinition(std::to_string(default_pipeline->id()),
                                        default_pipeline->getStructure())};

        PR_ASSERT(new_definition != nullptr,
                  "The newly created pipeline cannot already exist under a "
                  "different name.");

        auto new_id{default_pipeline->id()};

        auto default_material{this->createMaterial(
            MaterialType::DEFAULT_3D,
            std::format("DEFAULT PIPELINE {}", default_pipeline->id()))};

        default_material->setProperty(
            Presto::DefaultMaterialPropertyName::DIFFUSE_TEXTURE, fallback_tex);

        auto new_pipeline{
            std::make_unique<AllocatedPipeline>(
                AllocatedPipeline{.id = new_id,
                                  .pipeline = std::move(default_pipeline),
                                  .default_material{default_material}}),
        };

        impl_->pipelines.alloc(std::move(new_pipeline), new_id);
    }

    // Load the default quad (used for UI rendering)
    loadMesh(DefaultMeshes::Quad, PR_MESH_QUAD);
    loadMesh(DefaultMeshes::ZeroedSquare, PR_MESH_0_SQUARE);
};

RenderingManager::~RenderingManager() = default;

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

    AllocatedPipeline* allocated_pipeline = getPipeline(pipelineId);
    if (allocated_pipeline == nullptr) {
        PR_ERROR(
            "Unable to load mesh into pipeline #{}, as it is undefined. "
            "Skipping this mesh load.",
            pipelineId);
        return nullptr;
    }

    Pipeline* pipeline{allocated_pipeline->pipeline.get()};

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

[[nodiscard]] Ptr<Texture2D> RenderingManager::createTexture2D(
    const ImagePtr& image_ptr) {
    auto image{image_ptr->data()};

    // TODO: Add width/height validation here
    Ptr<Texture2D> new_texture{createTexture2D(image.width, image.height)};
    new_texture->write(image.bytes);

    return new_texture;
};

Ptr<Texture2D> RenderingManager::createTexture2D(Presto::size_t width,
                                                 Presto::size_t height) {
    texture_id_t new_id{impl_->texture_ids.generate()};
    Ptr<Texture2D> new_texture{impl_->texture_factory->new2D(width, height)};

    impl_->textures.emplace(new_id,
                            std::static_pointer_cast<Texture>(new_texture));

    return new_texture;
};

[[nodiscard]] Ptr<Texture2D> RenderingManager::createTexture2D(
    ImageData data, texture_id_t id) {
    // TODO: Add width/height validation here
    Ptr<Texture2D> new_texture{createTexture2D(data.width, data.height, id)};
    new_texture->write(std::move(data.bytes));

    return new_texture;
};

Ptr<Texture2D> RenderingManager::createTexture2D(Presto::size_t width,
                                                 Presto::size_t height,
                                                 texture_id_t id) {
    if (!impl_->texture_ids.reserve(id)) {
        PR_CORE_ERROR(
            "Unable to reserve texture id {} as it is already in use.");
        return nullptr;
    };

    Ptr<Texture2D> new_texture{impl_->texture_factory->new2D(width, height)};

    impl_->textures.emplace(id, std::static_pointer_cast<Texture>(new_texture));

    return new_texture;
};

void RenderingManager::update() {
    struct DrawStruct {
        ComponentPtr<RenderComponent> render;
        ComponentPtr<TransformComponent> transform;
    };

    // Update the global uniforms to the current camera
    renderer_->setCameraData(
        *(impl_->using_debug_cam
              ? impl_->cam_debug
              : impl_->cam_active_entity->getComponent<CameraComponent>()));

    auto& em{EntityManagerImpl::get()};

    auto mesh_draws{
        em.findAll() |
        std::views::transform([](const EntityPtr& entity) -> DrawStruct {
            return {.render = entity->getComponent<RenderComponent>(),
                    .transform = entity->getComponent<TransformComponent>()};
        }) |
        std::views::filter([](const DrawStruct& drawStruct) {
            return drawStruct.render != nullptr &&
                   drawStruct.transform != nullptr;
        })};

    std::ranges::for_each(mesh_draws, [this](const DrawStruct& drawStruct) {
        renderer_->setObjectData(
            {.transform = drawStruct.transform->getModelView()});

        for (const Ptr<ModelAsset>& model : drawStruct.render->getModels()) {
            for (const MeshDraw& draw : model->getDraws()) {
                if (draw.material == nullptr) {
                    PR_ERROR(
                        "No material available to render in 3D. Using the "
                        "fallback material.");

                    switchPipeline(PR_PIPELINE_DEFAULT_3D);
                    switchMaterial(impl_->current.pipeline->default_material);
                } else {
                    switchMaterial(draw.material);
                }

                MeshRegistrationData* data{impl_->mesh_registrations.find(
                    draw.mesh->registrationId())};
                PR_CORE_ASSERT(
                    data != nullptr,
                    "Mesh registrations can't be null at the draw phase.");

                renderer_->render(*data);
            }
        }

        for (QuadSubcomponent& quad : drawStruct.render->getQuads()) {
            // for (std::size_t i = 0; i < model.draws.size(); i++) {

            if (quad.material == nullptr) {
                PR_ERROR(
                    "No material available to render in 2D. Using the "
                    "fallback "
                    "material. ");
                continue;
            }

            switchMaterial(quad.material);

            auto* data{impl_->mesh_registrations.find(PR_MESH_0_SQUARE)};
            PR_CORE_ASSERT(data != nullptr,
                           "The default quad can not be null.");

            // TODO: Move this somewhere else so it isn't calculated 7 million
            // times
            quad.transform.scale = vec3{quad.width, quad.height, 0};

            renderer_->setObjectData(
                {.transform{drawStruct.transform->getModelView() *
                            quad.transform.asModelMat()}});

            renderer_->render(*data);
        }
    });

    auto canvas_draws{
        em.findAll() |
        std::views::transform(
            [](const EntityPtr& entity) -> ComponentPtr<CanvasComponent> {
                return entity->getComponent<CanvasComponent>();
            }) |
        std::views::filter(
            [](const auto& canvas) { return canvas != nullptr; })};

    AllocatedPipeline* ui_pipeline{getPipeline(PR_PIPELINE_DEFAULT_UI)};

    ui_pipeline->pipeline->bind();

    auto* quad_registration{impl_->mesh_registrations.find(PR_MESH_QUAD)};
    PR_CORE_ASSERT(quad_registration != nullptr,
                   "The default quad can not be null.");

    // Set pipeline to UI pipeline
    for (const ComponentPtr<CanvasComponent>& ptr : canvas_draws) {
        VisualExtents canvas_size{ptr->size()};

        impl_->cam_2d->setExtents(canvas_size);
        renderer_->setCameraData(GlobalUniforms{
            .view{mat4{1}},
            .projection{glm::ortho(0.F, static_cast<float>(canvas_size.width),
                                   static_cast<float>(canvas_size.height), 0.F,
                                   -1.F, 1.F)}});

        for (CanvasGroup& group : ptr->groups_) {
            // Render each canvasitem where it should be
            for (CanvasItem& canvasItem : group.items_) {
                if (canvasItem.texture() == nullptr) {
                    this->getTexture(PR_TEX_DIFFUSE_FALLBACK)->bind(2);
                } else {
                    canvasItem.texture()->bind(2);
                }

                // renderer_->setObjectData({.transform{}});

                canvasItem.buffer().bind(2);

                MeshRegistrationData* data{
                    impl_->mesh_registrations.find(canvasItem.meshId())};
                PR_CORE_ASSERT(
                    data != nullptr,
                    "Mesh registrations can't be null at the draw phase.");

                renderer_->render(*data);
            }
        }
    }

    // Unbind all bound resources
    impl_->current_pipeline_id = PR_PIPELINE_NONE;
    impl_->current.pipeline = nullptr;

    impl_->current.material.reset();

    PR_DEBUG_ONLY_CODE(
        // TODO: Reset the current camera
        renderer_->setCameraData(
            *(impl_->using_debug_cam
                  ? impl_->cam_debug
                  : impl_->cam_active_entity->getComponent<CameraComponent>())))

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

void RenderingManager::setMainCamera(const EntityPtr& mainCam) {
    PR_CORE_ASSERT(RenderingManager::initialised(),
                   "Unable to set camera when the RenderingManager is "
                   "uninitialised.")
    impl_->cam_active_entity = mainCam;
}

const PipelineStructure* RenderingManager::getPipelineStructure(
    pipeline_id_t id) const {
    auto transform_view{
        impl_->pipelines | std::views::values |
        std::views::transform([](const Allocated<AllocatedPipeline>& val)
                                  -> const PipelineStructure& {
            return val->pipeline->getStructure();
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
    PR_CORE_ASSERT(impl_->pipeline_builder != nullptr,
                   "The application must be initialised in order to "
                   "get the pipeline "
                   "builder.");

    return *impl_->pipeline_builder;
};

AllocatedPipeline* RenderingManager::getPipeline(pipeline_id_t id) const {
    auto* pipeline{impl_->pipelines.find(id)};
    if (pipeline == nullptr) {
        PR_WARN(
            "Pipeline of id {} could not be found in the RenderingManager. ",
            id);
    }

    return pipeline;
};

Ptr<Mesh> RenderingManager::loadMesh(MeshData meshData,
                                     mesh_registration_id_t customId) {
    PR_CORE_ASSERT(renderer_ != nullptr,
                   "The renderer must be initialised in order to load meshes.");

    auto pipelineId{meshData.pipeline_id};

    if (pipelineId == PR_PIPELINE_ANY) {
        pipelineId = PR_PIPELINE_DEFAULT_3D;
    }

    AllocatedPipeline* allocated_pipeline{nullptr};

    allocated_pipeline = getPipeline(pipelineId);
    if (allocated_pipeline == nullptr) {
        PR_ERROR(
            "Unable to load mesh into pipeline #{}, as it is undefined. "
            "Skipping this mesh load.",
            pipelineId);
        return nullptr;
    }

    if (impl_->current_pipeline_id != pipelineId) {
        allocated_pipeline->pipeline->bind();
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
        return nullptr;
    }

    bool success{renderer_->createMeshContext(
        *details, allocated_pipeline->pipeline->getStructure())};

    if (!success) {
        PR_ERROR("Unable to create mesh context in renderer.");
        return nullptr;
    }

    if (customId == PR_UNREGISTERED) {
        // Set it to the "define for me" allocation key
        customId = 0;
    }

    auto pair{impl_->mesh_registrations.alloc(std::move(details), customId)};

    mesh_registration_id_t registration_id{pair.first};
    pair.second->render_manager_id = registration_id;

    Ptr<Mesh> new_mesh{Ptr<Mesh>{new Mesh(pair.first)}};

    return new_mesh;
};

void RenderingManager::unloadMesh(Ptr<Mesh>&& ptr) {
    if (ptr == nullptr) {
        PR_CORE_ERROR(
            "An attempt was made to unload a nullptr Mesh. This will be "
            "ignored.");
        return;
    }

    if (!ptr.unique()) {
        PR_WARN("Unable to unload mesh with id {} as it is currently in use.",
                ptr->registrationId());
        return;
    }

    impl_->mesh_registrations.release(ptr->registrationId());
};

PR_DEBUG_ONLY_CODE(
    bool& RenderingManager::usingDebugCamera() {
        return impl_->using_debug_cam;
    }

    void RenderingManager::setUsingDebugCamera(bool isUsing) {
        impl_->using_debug_cam = isUsing;
    }  //
)

EntityPtr RenderingManager::getMainCamera() {
    return impl_->cam_active_entity;
    /*
auto cam{impl_->cam_active_entity->getComponent<CameraComponent>()};
PR_CORE_ASSERT(cam != nullptr, "The active camera must always be defined.");
return *cam;
    */
};

ComponentPtr<CameraComponent> RenderingManager::getDebugCamera() {
    return impl_->cam_debug;
    /*
auto cam{impl_->cam_debug_entity->getComponent<CameraComponent>()};
PR_CORE_ASSERT(cam != nullptr, "The debug camera must always be defined.");
return cam;
    */
};

void RenderingManager::switchPipeline(AllocatedPipeline* allocatedPipeline) {
    // Update pipeline if changed
    auto* pipeline = allocatedPipeline->pipeline.get();

    PR_ASSERT(pipeline != nullptr, "Unable to switch to null pipeline.");
    pipeline->bind();
    impl_->current.pipeline = allocatedPipeline;
    impl_->current_pipeline_id = pipeline->id();
}

void RenderingManager::switchPipeline(pipeline_id_t id) {
    auto* pipeline{getPipeline(id)};
    switchPipeline(pipeline);
}

void RenderingManager::switchMaterial(const MaterialPtr& material) {
    const auto pipeline_id{material->getPipelineId()};

    // Update pipeline if changed
    if (impl_->current.pipeline == nullptr ||
        impl_->current_pipeline_id != pipeline_id) {
        switchPipeline(pipeline_id);
    }

    // Update material if changed
    if (impl_->current.material.expired() ||
        impl_->current.material.lock() != material) {
        material->bindTo(*impl_->current.pipeline->pipeline);
        impl_->current.material = material;
    }
};

Ptr<Texture> RenderingManager::getDefaultTexture(const char* name) {
    if (name == DefaultMaterialPropertyName::DIFFUSE_TEXTURE) {
        return getTexture(PR_TEX_DIFFUSE_FALLBACK);
    }

    PR_ERROR("Unable to retrieve default texture \"{}\".", name);

    return nullptr;
};

Ptr<Texture> RenderingManager::getTexture(texture_id_t textureId) {
    if (!impl_->texture_ids.reserved(textureId)) {
        return nullptr;
    }
    return impl_->textures[textureId];
};

VisualExtents RenderingManager::framebufferSize() const {
    return renderer_->getExtents();
}

void RenderingManager::drawFromAllocation(MeshRegistrationData& data) {
    if (data.context_id == PR_UNREGISTERED) {
        PR_ERROR(
            "An unregistered allocation draw has been request. Ignoring this "
            "draw.");
        return;
    }
    renderer_->render(data);
}

Allocated<MeshRegistrationData> RenderingManager::allocateMeshRegistration(
    pipeline_id_t pipelineId, Presto::size_t vertexSize,
    Presto::size_t vertexCount, Presto::size_t indexCount) {
    PR_CORE_ASSERT(renderer_ != nullptr,
                   "The renderer must be initialised in order to load meshes.");

    if (pipelineId == PR_PIPELINE_ANY) {
        pipelineId = PR_PIPELINE_DEFAULT_3D;
    }

    AllocatedPipeline* allocated_pipeline = getPipeline(pipelineId);
    if (allocated_pipeline == nullptr) {
        PR_ERROR(
            "Unable to load mesh into pipeline #{}, as it is undefined. "
            "Skipping this mesh load.",
            pipelineId);
        return nullptr;
    }

    Pipeline* pipeline{allocated_pipeline->pipeline.get()};

    if (impl_->current_pipeline_id != pipelineId) {
        pipeline->bind();
        impl_->current_pipeline_id = pipelineId;
    }

    Presto::size_t vertex_buffer_size{vertexCount * vertexSize};
    Presto::size_t index_buffer_size{indexCount * sizeof(Index)};

    auto details{std::make_unique<MeshRegistrationData>(MeshRegistrationData{
        .render_manager_id{},
        .vertices = renderer_->createBuffer(Buffer::BufferType::VERTEX,
                                            vertex_buffer_size),
        .indices = renderer_->createBuffer(Buffer::BufferType::INDEX,
                                           index_buffer_size),
    })};

    bool success{renderer_->createMeshContext(
        *details, allocated_pipeline->pipeline->getStructure())};

    if (!success) {
        PR_ERROR("Unable to create mesh context in renderer.");
        return nullptr;
    }

    return details;
};

void RenderingManager::usePipeline(pipeline_id_t id) {
    getPipeline(id)->pipeline->bind();
}
}  // namespace Presto
