#include "OpenGLRenderer.h"

#include "OpenGLDrawManager/OpenGLDrawManager.h"

#include "Presto/Core/Constants.h"

#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Rendering/Renderer.h"

#include "Rendering/DefaultTextures.h"
#include "Rendering/Utils/RenderingUtils.h"

#include "OpenGLPipeline.h"
#include "OpenGLPipelineBuilder.h"
#include "OpenGLUniformBuffer.h"
#include "OpenGLVAO.h"

#include "DefaultShaders.h"

#include "Presto/Runtime/GLFWAppWindow.h"

#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Presto {
using PointType = float;

OpenGLRenderer::OpenGLRenderer(GLFWAppWindow* window) {
    this->setWindow(window);
    // glfwMakeContextCurrent(window->getWindowHandle());
    glewExperimental = GL_TRUE;

    auto res = glewInit();
    if (res != GLEW_OK) {
        PR_CORE_ERROR("{}", (const char*)(glewGetErrorString(res)));
        throw std::runtime_error("Unable to initialise GLEW.");
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);  // Smaller = closer

    setupDebugLogging();

    drawManager_ = std::make_unique<OpenGLDrawManager>();

    OpenGLPipelineBuilder builder{drawManager_.get()};

    PipelineStructure default_3d;
    PipelineStructure default_ui;

    builder.setShader(DEFAULT_VERTEX_SHADER, ShaderStage::VERTEX)
        .setShader(DEFAULT_FRAGMENT_SHADER, ShaderStage::FRAGMENT);
    default_3d = builder.build(PR_PIPELINE_DEFAULT_3D);

    builder.setShader(DEFAULT_UI_VERTEX_SHADER, ShaderStage::VERTEX)
        .setShader(DEFAULT_UI_FRAGMENT_SHADER, ShaderStage::FRAGMENT);
    default_ui = builder.build(PR_PIPELINE_DEFAULT_UI);

    drawManager_->setTexture(PR_DEFAULT_TEXTURE,
                             OpenGLTexture(DEFAULT_TEXTURE));

    auto global_uniforms_size{default_3d.uniform_blocks[0].size()};
    globalUniformBuffer_ =
        std::make_unique<OpenGLUniformBuffer>(global_uniforms_size);

    auto object_uniforms_size{default_3d.uniform_blocks[1].size()};

    objectUniformBuffer_ =
        std::make_unique<OpenGLUniformBuffer>(object_uniforms_size);
}

void OpenGLRenderer::nextFrame() {
    glfwSwapBuffers(static_cast<GLFWwindow*>(_glfwWindow->getWindowHandle()));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

OpenGLRenderer::~OpenGLRenderer() = default;

void GLAPIENTRY OpenGLRenderer::debugCallback(
    GLenum /*source*/, GLenum /*type*/, GLuint id, GLenum /*severity*/,
    GLsizei /*length*/, const GLchar* message, const void* /*userParam*/) {
    PR_CORE_TRACE("OpenGL Debug Message (ID: {}): {}", id, message);
}

void OpenGLRenderer::setupDebugLogging() {
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_DEBUG_OUTPUT);
    // Ignore messages that are not errors, warnings, or deprecated behavior
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0,
                          nullptr, GL_FALSE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM,
                          0, nullptr, GL_FALSE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0,
                          nullptr, GL_FALSE);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                          GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

    // Allow error, warning, and deprecated behavior messages
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE, 0,
                          nullptr, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
                          GL_DONT_CARE, 0, nullptr, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
                          GL_DONT_CARE, 0, nullptr, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE,
                          0, nullptr, GL_TRUE);
    glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PORTABILITY, GL_DONT_CARE,
                          0, nullptr, GL_TRUE);

    glDebugMessageCallback(debugCallback, nullptr);
};

void OpenGLRenderer::onFrameBufferResized() {
    auto extents{_glfwWindow->getFramebufferSize()};
    this->setExtents(extents);

    glViewport(0, 0, static_cast<GLsizei>(extents.width),
               static_cast<GLsizei>(extents.height));
}

renderer_mesh_id_t OpenGLRenderer::loadMesh(const ImportedMesh& mesh) {
    return this->drawManager_->createMeshContext(mesh);
};

void OpenGLRenderer::unloadMesh(renderer_mesh_id_t id) {
    this->drawManager_->destroyMeshContext(id);
};

void OpenGLRenderer::bindMeshToPipeline(renderer_mesh_id_t meshId,
                                        renderer_pipeline_id_t pipelineId) {
    MeshContext* context{drawManager_->getMeshContext(meshId)};
    PR_ASSERT(context != nullptr,
              "Unable to associate null mesh context with a pipeline.");

    if (auto vao{context->vao_map.find(pipelineId)};
        vao != context->vao_map.end()) {
        PR_WARN(
            "An attempt was made to re-bind mesh context {} to pipeline {}. "
            "Skipping this request.",
            meshId, pipelineId);
        return;
    }

    OpenGLPipeline* pipeline{drawManager_->getPipeline(pipelineId)};
    PR_ASSERT(context != nullptr,
              "Unable to associate a mesh context with a null pipeline.");

    context->vao_map.emplace(pipelineId,
                             OpenGLVAO(*context->buffer, *pipeline));
};

renderer_texture_id_t OpenGLRenderer::loadTexture(Presto::Image image) {
    return drawManager_->addTexture(image);
};

void OpenGLRenderer::unloadTexture(renderer_texture_id_t id) {
    drawManager_->removeTexture(id);
};

void OpenGLRenderer::render(renderer_mesh_id_t meshId) {
    PR_CORE_ASSERT(currentPipeline_ != nullptr,
                   "OpenGL is unable to render with no pipeline set.");

    // Get the relevant mesh
    MeshContext* mesh_context{drawManager_->getMeshContext(meshId)};

    PR_ASSERT(mesh_context != nullptr,
              "OpenGL render function received null mesh context.");

    // Get the VAO based on the current pipeline

    // auto* albedo{drawManager_->getTexture(imageId)};

    // Render
    this->draw(*mesh_context);
};

void OpenGLRenderer::draw(const MeshContext& context) {
    // If the uniforms have been updated or the material has been updated,
    // update the values
    if (this->isDirty()) {
        updateUniforms();
    }

    renderer_pipeline_id_t id{currentPipeline_->id()};

    auto vao_it{context.vao_map.find(id)};
    PR_ASSERT(
        vao_it != context.vao_map.end(),
        std::format(
            "No VAO found for combination of MeshContext and pipeline {}", id));

    vao_it->second.bind();

    const auto& details{context.buffer->getBufferDetails()};

    glDrawElements(details.draw_mode, details.index_count, GL_UNSIGNED_INT,
                   nullptr);

    /*
PR_CORE_ASSERT(
draw_data.mat_props.texture.isLoaded(),
"Draw data is used without being loaded in OpenGL Renderer.");
    */
};

void OpenGLRenderer::bindMaterial(const MaterialStructure& data) {
    // TODO: Extract pipeline system to be used from outside of the renderer
    /*
        auto id{data.materialType};

        OpenGLPipeline* pipeline{drawManager_->getPipeline(id)};

        PR_ASSERT(pipeline != nullptr,
                  "Unable to bind non-existant material from id {}.", id);

        if (pipeline != currentPipeline_) {
            currentPipeline_ = pipeline;
            PR_CORE_TRACE("Switching to new pipeline {}", fmt::ptr(pipeline));
        }
            */

    PR_ASSERT(
        currentPipeline_->accepts(data),
        "Pipeline does not accept material structure in OpenGL renderer.");

    /*OpenGLTexture* diffuse{drawManager_->getTexture(data.diffuseTexture)};*/
    /*if (diffuse == nullptr) {*/
    /*    PR_WARN("No diffuse texture was available. Using default texture.");*/
    /*} else {*/
    /*    diffuse = drawManager_->getTexture(PR_DEFAULT_TEXTURE);*/
    /**/
    /*    // TODO: Move this to a place where its always checked instead of*/
    /*    // only when it's needed*/
    /*    PR_CORE_ASSERT(diffuse != nullptr,*/
    /*                   "The default texture could not be found.");*/
    /*}*/

    currentPipeline_->setProperties(data);
    currentPipeline_->bind();

    setDirty();
};

void OpenGLRenderer::unbindMaterial() {
    currentPipeline_->unbind();
    currentPipeline_ = nullptr;
};

void OpenGLRenderer::updateUniforms() {
    PR_CORE_ASSERT(currentPipeline_ != nullptr,
                   "Unable to setup uniforms on a null material.");

    ByteArray bytes(sizeof(globalUniformBuffer_));

    globalUniformBuffer_->write(bytes);

    bytes = ByteArray(sizeof(objectUniformBuffer_));

    objectUniformBuffer_->write(bytes);

    setDirty(false);
};

void OpenGLRenderer::usePipeline(renderer_pipeline_id_t pipelineId) {
    OpenGLPipeline* pipeline{drawManager_->getPipeline(pipelineId)};

    PR_ASSERT(
        pipeline != nullptr,
        std::format("A non-existant pipeline {} can't be used.", pipelineId));

    pipeline->bind();
};

Allocated<PipelineBuilder> OpenGLRenderer::getPipelineBuilder() {
    return Allocated<PipelineBuilder>{
        new OpenGLPipelineBuilder{this->drawManager_.get()}};
}

std::vector<PipelineStructure> OpenGLRenderer::getPipelineStructures() const {
    for (const auto& x : drawManager_->getPipelines()};
}  // namespace Presto
