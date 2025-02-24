#include "Presto/Modules/RenderingManager.h"
#include "Presto/Components/Renderable/ModelComponent.h"
#include "Presto/Core/Types.h"

#include "Presto/Modules/EntityManager.h"

#include "Presto/Components/CameraComponent.h"
#include "Presto/Components/TransformComponent.h"
#include "Presto/Rendering/Pipeline.h"
#include "Rendering/Renderer.h"

// #include "Rendering/Vulkan/VulkanRenderer.h"

#include "Presto/Runtime/GLFWAppWindow.h"

#include "Utils/IDGenerator.h"

#include "Presto/Modules/AssetManager.h"

#include "Memory/AllocatorTypes.h"

namespace Presto {

constexpr auto PR_MIN_USER_ID = 10;

RENDER_LIBRARY RenderingManager::_library = UNSET;
GLFWAppWindow* RenderingManager::_window = nullptr;

struct RenderingManager::Impl {
    IDGenerator<mesh_id_t> mesh_ids;
    IDGenerator<material_id_t> material_ids;
    IDGenerator<texture_id_t> texture_ids;

    // std::map<renderer_mesh_id_t, MeshContext> bufferMap_;

    pipeline_allocator_t pipelines;

    std::map<texture_id_t, Texture> textures;

    Allocated<TextureFactory> textureFactory_;
    Allocated<PipelineBuilder> pipelineBuilder_;
};

RenderingManager::RenderingManager(RENDER_LIBRARY library,
                                   GLFWAppWindow* window,
                                   CameraComponent& defaultCamera)
    : activeCamera_(defaultCamera) {
    Allocated<Renderer> renderer_ = Renderer::create(library, window);
    impl_ = std::make_unique<Impl>();

    impl_->textureFactory_ = renderer_->getTextureFactory();
};

renderer_mesh_id_t RenderingManager::loadMesh(const ImportedMesh& mesh) {
    auto new_id{impl_->mesh_ids.generate()};

    auto vertices{mesh.attributes};
    Presto::size_t buffer_size{vertices.size()};

    static_assert(
        false,
        "Fix this to actually make the vertices and then get the proper size.");

    MeshRegistrationData details{
        .id = new_id,
        .vertices =
            renderer_->createBuffer(Buffer::BufferType::VERTEX, buffer_size),
        .indices = renderer_->createBuffer(Buffer::BufferType::INDEX,
                                           mesh.indices.size() * sizeof(Index)),
    };

    return new_id;
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

    impl_->textures.emplace(new_id, new_texture);

    return new_texture;
};

void RenderingManager::update() {
    struct DrawStruct {
        ComponentPtr<ModelComponent> model;
        ComponentPtr<TransformComponent> transform;
    };

    // Update the global uniforms to the current camera
    renderer_->setCameraData(activeCamera_);

    auto& em{EntityManager::get()};

    auto mesh_draws{em.findAll() |
                    std::views::transform([](entity_ptr entity) -> DrawStruct {
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

            MaterialStructure mat_data{};

            if (material != nullptr) {
                mat_data = material.lock();
            } else if (!mesh->defaultMaterial_.expired()) {
                mat_data = mesh->defaultMaterial_.lock()->getStructure();
            }

            renderer_->bindMaterial(mat_data);

            auto mesh_id{mesh->renderId_};
            renderer_->render(mesh_id);
        }
    });

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
    auto image_id{renderer_->createTexture(image.getImage())};
    image.renderId_ = image_id;
};

PipelineStructure* RenderingManager::getPipelineStructure(
    renderer_pipeline_id_t id) const {
    auto structures{renderer_->getPipelineStructures()};

    if (auto found{std::ranges::find_if(
            structures,
            [id](const PipelineStructure& structure) -> bool {
                return structure.pipeline_id == id;
            })};
        found != structures.end()) {
        return found.base();
    };

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

    return std::make_shared<MaterialInstance>(definition);
};

Ptr<MaterialInstance> RenderingManager::getMaterial(
    const Presto::string& name) {
    // TODO: Implement

    return nullptr;
};

PipelineBuilder& RenderingManager::getPipelineBuilder() {
    PR_CORE_ASSERT(
        impl_->pipelineBuilder_ != nullptr,
        "The application must be initialised in order to get the pipeline "
        "builder.");

    return *impl_->pipelineBuilder_;
};

/*
MeshContext* RenderingManager::getMeshContext(renderer_mesh_id_t id) {
    auto mesh{bufferMap_.find(id)};

    return (mesh == bufferMap_.end()) ? nullptr : &(mesh->second);
}

void RenderingManager::removeTexture(renderer_texture_id_t id) {
    auto erased{textureMap_.erase(id)};
    if (erased == 0) {
        PR_CORE_WARN(
            "A delete was requested for a non-existant texture with ID {} "
            "in "
            "the OpenGL Draw Manager.",
            id);
    }
};

void RenderingManager::destroyMeshContext(renderer_mesh_id_t id) {
    auto erased{bufferMap_.erase(id)};
    if (erased == 0) {
        PR_CORE_WARN(
            "A delete was requested for a non-existant mesh with ID {} in "
            "the OpenGL Draw Manager.",
            id);
    }
};

OpenGLTexture* RenderingManager::getTexture(renderer_texture_id_t id) {
    if (id == UNREGISTERED_RENDER_DATA_ID) {
        PR_WARN(
            "An unregistered texture has been requested in the draw "
            "manager. Using the default texture.");

        return &textureMap_[PR_DEFAULT_TEXTURE];
    }

    auto texture{textureMap_.find(id)};

    return texture == textureMap_.end() ? nullptr : &(texture->second);
};

renderer_mesh_id_t RenderingManager::createMeshContext(
    const ImportedMesh& mesh) {
    auto buffer{std::make_unique<OpenGLBuffer>(mesh.attributes,
                                               mesh.vertex_count, mesh.bytes,
                                               mesh.indices, mesh.draw_mode)};
    MeshContext context(std::move(buffer));

    auto new_key{++currentKey_};
    const auto insertion{bufferMap_.emplace(new_key, buffer)};

    PR_CORE_ASSERT(insertion.second,
                   "Presto failed to insert RenderGroup into the render list.");

    PR_CORE_TRACE("Added new RenderGroup to the render list.");

    return new_key;
};

renderer_texture_id_t RenderingManager::addTexture(const Presto::Image& image) {
    OpenGLTexture tex{image};

    auto new_key{++currentKey_};
    textureMap_.emplace(new_key, std::move(tex));

    return new_key;
};

OpenGLPipeline* RenderingManager::getPipeline(renderer_pipeline_id_t id) {
    auto pipeline{pipelineMap_.find(id)};

    return (pipeline == pipelineMap_.end()) ? nullptr : &(pipeline->second);
}
*/

/*
PipelineStructure RenderingManager::addPipeline(OpenGLPipeline&& pipeline,
                                                renderer_pipeline_id_t id) {
    if (id == ANY_PIPELINE) {
        id = currentKey_++;
    }

    PipelineStructure structure{pipeline.getStructure()};

    if ()

        // TODO: Make this only affect pipelines with the buffers
        pipeline->setUniformBlock(0, *globalUniformBuffer_.get());
    pipeline->setUniformBlock(1, *objectUniformBuffer_.get());

    pipelineMap_.emplace(id, std::move(pipeline));
};
*/

}  // namespace Presto
