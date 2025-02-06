#include "OpenGLRenderer.h"

#include "OpenGLDrawManager/OpenGLDrawManager.h"

#include "Presto/Core/Constants.h"
#include "Presto/Rendering/MaterialData.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Rendering/Renderer.h"
#include "Rendering/OpenGL/OpenGLPipeline.h"
#include "Rendering/OpenGL/OpenGLVAO.h"
#include "Rendering/Utils/RenderingUtils.h"

#include "OpenGLDrawManager/DefaultShaders.h"

#include "Presto/Runtime/GLFWAppWindow.h"

#include <glm/gtc/type_ptr.hpp>

#include "Modules/ModelLoader.h"

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

    ShaderPtr default_shader{std::make_shared<OpenGLShader>()};
    default_shader->setShader(DEFAULT_VERTEX_SHADER, ShaderStage::VERTEX)
        .setShader(DEFAULT_FRAGMENT_SHADER, ShaderStage::FRAGMENT)
        .linkShaderProgram();

    ShaderPtr ui_shader{std::make_shared<OpenGLShader>()};
    default_shader->setShader(DEFAULT_UI_VERTEX_SHADER, ShaderStage::VERTEX)
        .setShader(DEFAULT_UI_FRAGMENT_SHADER, ShaderStage::FRAGMENT)
        .linkShaderProgram();

    drawManager_->setPipeline(PR_PIPELINE_DEFAULT_3D,
                              OpenGLPipeline{default_shader});
    drawManager_->setPipeline(PR_PIPELINE_DEFAULT_UI,
                              OpenGLPipeline{ui_shader});

    drawManager_->setTexture(PR_DEFAULT_TEXTURE,
                             OpenGLTexture{Presto::Image{.width = 2,
                                                         .height = 2,
                                                         .bytes = {
                                                             0,
                                                             0,
                                                             0,
                                                             255,

                                                             255,
                                                             255,
                                                             255,
                                                             255,

                                                             255,
                                                             255,
                                                             255,
                                                             255,

                                                             0,
                                                             0,
                                                             0,
                                                             255,
                                                         }}});

    /*
            constexpr auto POS_VECTOR_SIZE = 3;
            constexpr auto TRIANGLE_POINT_COUNT = POS_VECTOR_SIZE * 3;

            PointType triangle[] = {0.0f, 0.5f,  0.0f,  0.5f, -0.5f,
                                    0.0f, -0.5f, -0.5f, 0.0f};
                                                                    */
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

    auto vao_it{context.vao_map.find(currentPipeline_->id_)};
    PR_ASSERT(vao_it != context.vao_map.end(),
              std::format(
                  "No VAO found for combination of MeshContext and pipeline {}",
                  currentPipeline_->id_));

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
    auto id{data.materialType};

    OpenGLPipeline* pipeline{drawManager_->getPipeline(id)};

    PR_ASSERT(pipeline != nullptr,
              "Unable to bind non-existant material from id {}.", id);

    if (pipeline != currentPipeline_) {
        currentPipeline_ = pipeline;
        PR_CORE_TRACE("Switching to new pipeline {}", fmt::ptr(pipeline));
    }

    OpenGLTexture* diffuse{drawManager_->getTexture(data.diffuseTexture)};
    if (diffuse == nullptr) {
        PR_WARN("No diffuse texture was available. Using default texture.");
    } else {
        diffuse = drawManager_->getTexture(PR_DEFAULT_TEXTURE);

        // TODO: Move this to a place where its always checked instead of
        // only when it's needed
        PR_CORE_ASSERT(diffuse != nullptr,
                       "The default texture could not be found.");
    }

    currentPipeline_->setDiffuse(diffuse);

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

    ShaderPtr shader{currentPipeline_->getShader()};

    shader->setUniform("view", globalUniforms_.view);
    shader->setUniform("projection", globalUniforms_.projection);
    // TODO: Make this a one-off function call
    shader->setUniform("sampler1", std::int8_t(0));

    shader->setUniform("transform", objectUniforms_.transform);

    setDirty(false);
};
}  // namespace Presto
