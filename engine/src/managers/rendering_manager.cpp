module presto.internal.managers.rendering_manager;

import presto.utils;
import presto.utils.erased_bytes;

import presto.assets.model;

import presto.objects;
import presto.objects.components;
import presto.types.rendering;

import presto.core.assert;

import presto.internal.events;
import presto.internal.glfw;
import presto.internal.rendering;
import presto.internal.managers;
import presto.internal.defaults;

import std;
import glm;

#include "presto/platform.h"

namespace Pr {

// constexpr auto PR_MIN_USER_PIPELINE_ID = 10;

RENDER_LIBRARY RenderingManager::_library = UNSET;
GLFWAppWindow* RenderingManager::_window = nullptr;

struct RenderingManager::Impl {
    IDGenerator<material_id_t> material_ids;
    IDGenerator<texture_id_t> texture_ids;

    std::vector<Pr::Ptr<Pr::MaterialInstanceImpl>> materials;

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
        Pr::CoreLog(
            WARN,
            "Attempted to initialise the default pipelines when they are "
            "already set up. Ignoring this request.");
        return;
    }

    AssetManager& am{AssetManager::get()};

    Renderer::AllocatedPipelineList default_pipelines{
        renderer_->createDefaultPipelines()};

    Pr::Ptr<Pr::Texture> fallback_tex{
        this->createTexture2D(DEFAULT_TEXTURE_DATA, PR_TEX_DIFFUSE_FALLBACK)};
    Pr::Ptr<Pr::Texture> flat_shading_tex{
        this->createTexture2D(DEFAULT_TEXTURE_DATA, PR_TEX_DIFFUSE_FLAT)};

    // Check that all of them loaded correctly
    std::ranges::for_each(
        default_pipelines, [](const Allocated<Pipeline>& default_pipeline) {
            Pr::CoreAssert(
                default_pipeline != nullptr,
                "The default pipelines must be initialised correctly "
                "when being used, not nullptr.");
        });

    for (Allocated<Pipeline>& default_pipeline : default_pipelines) {
        auto new_definition{
            am.createMaterialDefinition(std::to_string(default_pipeline->id()),
                                        default_pipeline->getStructure())};

        Pr::Assert(new_definition != nullptr,
                   "The newly created pipeline cannot already exist under a "
                   "different name.");

        auto new_id{default_pipeline->id()};

        auto default_material{this->createMaterial(
            MaterialType::DEFAULT_3D,
            std::format("DEFAULT PIPELINE {}", default_pipeline->id()))};

        default_material->setProperty(
            Pr::DefaultMaterialPropertyName::DIFFUSE_TEXTURE, fallback_tex);

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
    Pr::CoreAssert(renderer_ != nullptr,
                   "The renderer must be initialised in order to load meshes.");

    if (pipelineId == PR_PIPELINE_ANY) {
        pipelineId = PR_PIPELINE_DEFAULT_3D;
    }

    AllocatedPipeline* allocated_pipeline = getPipeline(pipelineId);
    if (allocated_pipeline == nullptr) {
        Pr::CoreLog(
            ERROR,
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

    Pr::size_t vertex_buffer_size{vertices.size() * sizeof(T)};
    Pr::size_t index_buffer_size{indices.size() * sizeof(Index)};

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
    Pr::CoreAssert(_library != UNSET,
                   "Unable to initialise the RenderingManager with an "
                   "unset graphics library.");
    Pr::CoreAssert(_window != nullptr,
                   "Unable to initialise the RenderingManager with a null "
                   "Window handle.");

    instance_ = std::unique_ptr<RenderingManager>(new RenderingManager(
        RenderingManager::_library, RenderingManager::_window));
}

[[nodiscard]] Ptr<Texture2D> RenderingManager::createTexture2D(
    const Pr::Ptr<Pr::ImageAsset>& image_ptr) {
    auto image{image_ptr->data()};

    // TODO: Add width/height validation here
    Ptr<Texture2D> new_texture{createTexture2D(image.width, image.height)};
    new_texture->write(image.bytes);

    return new_texture;
};

Ptr<Texture2D> RenderingManager::createTexture2D(Pr::size_t width,
                                                 Pr::size_t height) {
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

Ptr<Texture2D> RenderingManager::createTexture2D(Pr::size_t width,
                                                 Pr::size_t height,
                                                 texture_id_t id) {
    if (!impl_->texture_ids.reserve(id)) {
        Pr::CoreLog(ERROR,
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
                    Pr::CoreLog(
                        ERROR,
                        "No material available to render in 3D. Using the "
                        "fallback material.");

                    switchPipeline(PR_PIPELINE_DEFAULT_3D);
                    switchMaterial(impl_->current.pipeline->default_material);
                } else {
                    switchMaterial(draw.material);
                }

                MeshRegistrationData* data{impl_->mesh_registrations.find(
                    draw.mesh->registrationId())};
                Pr::CoreAssert(
                    data != nullptr,
                    "Mesh registrations can't be null at the draw phase.");

                renderer_->render(*data);
            }
        }

        for (QuadSubcomponent& quad : drawStruct.render->getQuads()) {
            // for (Pr::size_t i = 0; i < model.draws.size(); i++) {

            if (quad.material == nullptr) {
                Pr::CoreLog(ERROR,
                            "No material available to render in 2D. Using the "
                            "fallback "
                            "material. ");
                continue;
            }

            switchMaterial(quad.material);

            auto* data{impl_->mesh_registrations.find(PR_MESH_0_SQUARE)};
            Pr::CoreAssert(data != nullptr,
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
    Pr::CoreAssert(quad_registration != nullptr,
                   "The default quad can not be null.");

    // Set pipeline to UI pipeline
    for (const ComponentPtr<CanvasComponent>& ptr : canvas_draws) {
        VisualExtents canvas_size{ptr->size()};

        impl_->cam_2d->setExtents(canvas_size);
        renderer_->setCameraData(GlobalUniforms{
            .view{mat4{1}},
            .projection{glm::gtc::ortho(
                0.F, static_cast<float>(canvas_size.width),
                static_cast<float>(canvas_size.height), 0.F, -1.F, 1.F)}});

        for (CanvasGroup& group : ptr->groups()) {
            // Render each canvasitem where it should be
            for (CanvasItem& canvasItem : group.items()) {
                if (canvasItem.texture() == nullptr) {
                    this->getTexture(PR_TEX_DIFFUSE_FALLBACK)->bind(2);
                } else {
                    canvasItem.texture()->bind(2);
                }

                // renderer_->setObjectData({.transform{}});

                canvasItem.buffer().bind(2);

                MeshRegistrationData* data{
                    impl_->mesh_registrations.find(canvasItem.meshId())};
                Pr::CoreAssert(
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
    Pr::CoreAssert(!RenderingManager::initialised(),
                   "Unable to set render library while the renderer is "
                   "already initialised.");

    RenderingManager::_library = library;
}
void RenderingManager::setWindow(GLFWAppWindow* window) {
    Pr::CoreAssert(!RenderingManager::initialised(),
                   "Unable to set window surface while the renderer is "
                   "already initialised.");

    RenderingManager::_window = window;
}

void RenderingManager::setMainCamera(const EntityPtr& mainCam) {
    Pr::CoreAssert(RenderingManager::initialised(),
                   "Unable to set camera when the RenderingManager is "
                   "uninitialised.");
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
    Pr::size_t size) {
    return renderer_->createUniformBuffer(size);
};

Ptr<MaterialInstance> RenderingManager::createMaterial(MaterialType type,
                                                       Pr::string name) {
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
    Pr::CoreAssert(
        definition != nullptr,
        "A default pipeline can't have a null shared pointer to it.");

    auto new_instance{std::make_shared<MaterialInstanceImpl>(definition)};
    new_instance->setName(std::move(name));

    impl_->materials.push_back(new_instance);

    return new_instance;
};

Ptr<MaterialInstance> RenderingManager::findMaterial(const Pr::string& name) {
    if (auto found{std::ranges::find_if(
            impl_->materials,
            [name](const Pr::Ptr<Pr::MaterialInstance>& val) {
                return val->name() == name;
            })};
        found != impl_->materials.end()) {
        return *found;
    }

    return nullptr;
};

PipelineBuilder& RenderingManager::getPipelineBuilder() {
    Pr::CoreAssert(impl_->pipeline_builder != nullptr,
                   "The application must be initialised in order to "
                   "get the pipeline "
                   "builder.");

    return *impl_->pipeline_builder;
};

AllocatedPipeline* RenderingManager::getPipeline(pipeline_id_t id) const {
    auto* pipeline{impl_->pipelines.find(id)};
    if (pipeline == nullptr) {
        Pr::CoreLog(
            WARN,
            "Pipeline of id {} could not be found in the RenderingManager. ",
            id);
    }

    return pipeline;
};

Ptr<Mesh> RenderingManager::loadMesh(MeshData meshData,
                                     mesh_registration_id_t customId) {
    Pr::CoreAssert(renderer_ != nullptr,
                   "The renderer must be initialised in order to load meshes.");

    auto pipelineId{meshData.pipeline_id};

    if (pipelineId == PR_PIPELINE_ANY) {
        pipelineId = PR_PIPELINE_DEFAULT_3D;
    }

    AllocatedPipeline* allocated_pipeline{nullptr};

    allocated_pipeline = getPipeline(pipelineId);
    if (allocated_pipeline == nullptr) {
        Pr::CoreLog(
            ERROR,
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
        Pr::CoreLog(ERROR, "Unable to create mesh registration from MeshData.");
        return nullptr;
    }

    bool success{renderer_->createMeshContext(
        *details, allocated_pipeline->pipeline->getStructure())};

    if (!success) {
        Pr::CoreLog(ERROR, "Unable to create mesh context in renderer.");
        return nullptr;
    }

    if (customId == PR_UNREGISTERED) {
        // Set it to the "define for me" allocation key
        customId = 0;
    }

    auto pair{impl_->mesh_registrations.alloc(std::move(details), customId)};

    mesh_registration_id_t registration_id{pair.first};
    pair.second->render_manager_id = registration_id;

    Ptr<Mesh> new_mesh{std::make_shared<Mesh>(registration_id)};

    return new_mesh;
};

void RenderingManager::unloadMesh(Ptr<Mesh>&& ptr) {
    if (ptr == nullptr) {
        Pr::CoreLog(
            ERROR,
            "An attempt was made to unload a nullptr Mesh. This will be "
            "ignored.");
        return;
    }

    // TODO: Reimplement this with proper reference tracking
    /*
if (!ptr.unique()) {
    Pr::CoreLog(
        WARN, "Unable to unload mesh with id {} as it is currently in use.",
        ptr->registrationId());
    return;
}
    */

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
Pr::CoreAssert(cam != nullptr, "The active camera must always be defined.");
return *cam;
    */
};

ComponentPtr<CameraComponent> RenderingManager::getDebugCamera() {
    return impl_->cam_debug;
    /*
auto cam{impl_->cam_debug_entity->getComponent<CameraComponent>()};
Pr::CoreAssert(cam != nullptr, "The debug camera must always be defined.");
return cam;
    */
};

void RenderingManager::switchPipeline(AllocatedPipeline* allocatedPipeline) {
    // Update pipeline if changed
    auto* pipeline = allocatedPipeline->pipeline.get();

    Pr::Assert(pipeline != nullptr, "Unable to switch to null pipeline.");
    pipeline->bind();
    impl_->current.pipeline = allocatedPipeline;
    impl_->current_pipeline_id = pipeline->id();
}

void RenderingManager::switchPipeline(pipeline_id_t id) {
    auto* pipeline{getPipeline(id)};
    switchPipeline(pipeline);
}

void RenderingManager::switchMaterial(
    const Pr::Ptr<Pr::MaterialInstance>& material) {
    const auto pipeline_id{material->getPipelineId()};

    // Update pipeline if changed
    if (impl_->current.pipeline == nullptr ||
        impl_->current_pipeline_id != pipeline_id) {
        switchPipeline(pipeline_id);
    }

    // Update material if changed
    if (impl_->current.material.expired() ||
        impl_->current.material.lock() != material) {
        std::dynamic_pointer_cast<MaterialInstanceImpl>(material)->bindTo(
            *impl_->current.pipeline->pipeline);
        impl_->current.material = material;
    }
};

Ptr<Texture> RenderingManager::getDefaultTexture(const char* name) {
    if (name == DefaultMaterialPropertyName::DIFFUSE_TEXTURE) {
        return getTexture(PR_TEX_DIFFUSE_FALLBACK);
    }

    Pr::CoreLog(ERROR, "Unable to retrieve default texture \"{}\".", name);

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
        Pr::CoreLog(
            ERROR,
            "An unregistered allocation draw has been request. Ignoring this "
            "draw.");
        return;
    }
    renderer_->render(data);
}

Allocated<MeshRegistrationData> RenderingManager::allocateMeshRegistration(
    pipeline_id_t pipelineId, Pr::size_t vertexSize, Pr::size_t vertexCount,
    Pr::size_t indexCount) {
    Pr::CoreAssert(renderer_ != nullptr,
                   "The renderer must be initialised in order to load meshes.");

    if (pipelineId == PR_PIPELINE_ANY) {
        pipelineId = PR_PIPELINE_DEFAULT_3D;
    }

    AllocatedPipeline* allocated_pipeline = getPipeline(pipelineId);
    if (allocated_pipeline == nullptr) {
        Pr::CoreLog(
            ERROR,
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

    Pr::size_t vertex_buffer_size{vertexCount * vertexSize};
    Pr::size_t index_buffer_size{indexCount * sizeof(Index)};

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
        Pr::CoreLog(ERROR, "Unable to create mesh context in renderer.");
        return nullptr;
    }

    return details;
};

void RenderingManager::usePipeline(pipeline_id_t id) {
    getPipeline(id)->pipeline->bind();
}

MaterialInstanceImpl::MaterialInstanceImpl(
    const MaterialDefinitionPtr& definition) {
    this->definition_ = definition;
    this->structure_ = definition->uniformLayout();

    RenderingManager& rm{RenderingManager::get()};

    // Allocate a buffer for each uniform block (needed for hotswap)
    this->uniformBuffers_.resize(this->structure_.blocks.size());
    for (Pr::size_t i{0}; i < this->structure_.blocks.size(); i++) {
        const UniformBlock& block{this->structure_.blocks[i]};

        this->uniformBuffers_[i] = {
            .bind_point = block.bind_point,
            .buffer = rm.createUniformBuffer(block.size())};

        for (const UniformBinding& binding : block.bindings) {
            propertyLookup_[binding.name] = {
                .binding{binding},
                .data_index = i,
            };
        }
    }

    Pr::size_t naked_binding_count{this->structure_.bindings.size()};

    this->uniformBindings_.resize(naked_binding_count);
    for (Pr::size_t i = 0; i < naked_binding_count; i++) {
        const UniformBinding& binding{this->structure_.bindings[i]};
        propertyLookup_[binding.name] = {.binding = binding, .data_index = i};

        this->uniformBindings_[i] = {
            .location = static_cast<Pr::uint8_t>(binding.location),
            .data_type = binding.data_type,
            .data = ErasedBytes{ByteArray(binding.size())}};

        if (binding.data_type == UniformVariableType::TEXTURE) {
            this->uniformBindings_[i].data.reset(
                static_cast<ImportTypeOf<UniformVariableType::TEXTURE>>(
                    this->textures_.size()));

            TexturePtr tex{nullptr};

            if (binding.name == DefaultMaterialPropertyName::DIFFUSE_TEXTURE) {
                tex = rm.getTexture(PR_TEX_DIFFUSE_FALLBACK);
            }

            this->textures_.push_back(tex);
        }
    }
}

MaterialInstanceImpl::~MaterialInstanceImpl() = default;

MaterialInstanceImpl::PropertyDetails* MaterialInstanceImpl::getBinding(
    const Pr::string& name) {
    if (auto val{propertyLookup_.find(name)}; val != propertyLookup_.end()) {
        return &(val->second);
    }
    return nullptr;
};

MaterialInstance& MaterialInstanceImpl::setName(Pr::string newName) {
    name_ = std::move(newName);
    return *this;
};

ErasedBytes& MaterialInstanceImpl::getUniformDataStore(Pr::size_t index) {
    return uniformBindings_[index].data;
};

UniformBuffer& MaterialInstanceImpl::getUniformBuffer(Pr::size_t index) {
    return *uniformBuffers_[index].buffer;
};

const UniformLayout& MaterialInstanceImpl::getUniformStructure() const {
    return structure_;
};

pipeline_id_t MaterialInstanceImpl::getPipelineId() const {
    return definition_->pipelineId();
};

Pr::string MaterialInstanceImpl::name() const { return name_; };

void MaterialInstanceImpl::bindTo(Pipeline& pipeline) const {
    // Bind each block
    for (const UniformBufferExtra& buffer_details : uniformBuffers_) {
        pipeline.setUniformBlock(buffer_details.bind_point,
                                 *buffer_details.buffer);
    }
    // Bind each singular uniform

    for (const UniformBindingExtra& binding : uniformBindings_) {
#define SWITCH_CASE(type)                                          \
    case type:                                                     \
        pipeline.setUniform(binding.location,                      \
                            binding.data.as<GPUTypeOf<(type)>>()); \
        break

        switch (binding.data_type) {
            SWITCH_CASE(UniformVariableType::INT);
            SWITCH_CASE(UniformVariableType::UINT);
            SWITCH_CASE(UniformVariableType::FLOAT);
            SWITCH_CASE(UniformVariableType::VEC2);
            SWITCH_CASE(UniformVariableType::VEC3);
            SWITCH_CASE(UniformVariableType::VEC4);
            SWITCH_CASE(UniformVariableType::MAT4);

            case UniformVariableType::TEXTURE: {
                auto texture_index{binding.data.as<Pr::uint8_t>()};
                const TexturePtr& texture{textures_[texture_index]};

                if (texture == nullptr) {
                    /*
Pr::CoreLog(TRACE,
    "No texture specified at location {} in material "
    "instance. Using fallback instead.",
    binding.location);
                            */

                    continue;
                }

                texture->bind(binding.location);

                break;
            }

            default:
                Pr::CoreLog(
                    ERROR,
                    "Unable to determine type of binding data. Skipping "
                    "this "
                    "write.");
                break;
        }
#undef SWITCH_CASE
    }
};

void MaterialInstanceImpl::setFromImport(
    const ImportedMaterial& imported_material,
    std::vector<TexturePtr>& texturePtrs) {
    for (const auto& value : imported_material.values) {
        if (value.data_type == UniformVariableType::TEXTURE) {
            auto texture_index{
                value.data.as<ImportTypeOf<UniformVariableType::TEXTURE>>()};
            this->setProperty(value.name, texturePtrs[texture_index]);
        } else {
            this->setProperty(value.name, value.data);
        }
    }
};

MaterialInstance& MaterialInstanceImpl::setProperty(Pr::string name,
                                                    const Ptr<Texture>& data) {
    PropertyDetails* details{getBinding(name)};

    if (details == nullptr) {
        Pr::CoreLog(
            WARN,
            "Unable to find texture \"{}\" in MaterialInstance of pipeline "
            "{}. "
            "Skipping this write.",
            name, this->getPipelineId());
        return *this;
    }

    if (details->binding.data_type != UniformVariableType::TEXTURE) {
        Pr::CoreLog(
            ERROR,
            "Unable to write Texture value to \"{}\" in pipeline {}. (It "
            "is "
            "not a texture type). Skipping this write.",
            name, this->getPipelineId());
        return *this;
    }

    auto texture_index{getUniformDataStore(details->data_index)
                           .as<ImportTypeOf<UniformVariableType::TEXTURE>>()};
    Pr::CoreAssert(
        texture_index < textures_.size(),
        std::format("Texture index {} must be in bounds for array of size {}.",
                    texture_index, textures_.size()));

    textures_[texture_index] = data;

    return *this;
};

}  // namespace Pr
