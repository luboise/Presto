#include "OpenGLRenderer.h"

#include "Presto/Core/Constants.h"

#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/RenderTypes.h"

#include "Rendering/OpenGL/OpenGLBuffer.h"
#include "Rendering/Renderer.h"

#include "Rendering/DefaultTextures.h"
#include "Rendering/Utils/RenderingUtils.h"

#include "OpenGLPipeline.h"
#include "OpenGLPipelineBuilder.h"
#include "OpenGLUniformBuffer.h"
#include "OpenGLVAO.h"

#include "OpenGLTextureFactory.h"

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

    setupDebugLogging();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);  // Smaller = closer

    globalUniformBuffer_ =
        std::make_unique<OpenGLUniformBuffer>(sizeof(GlobalUniforms));

    objectUniformBuffer_ =
        std::make_unique<OpenGLUniformBuffer>(sizeof(ObjectUniforms));
}

Renderer::AllocatedPipelineList OpenGLRenderer::createDefaultPipelines() {
    AllocatedPipelineList pipelines(2);

    OpenGLPipelineBuilder builder{};

    builder.setId(PR_PIPELINE_DEFAULT_3D)
        .setShader(DEFAULT_VERTEX_SHADER, ShaderStage::VERTEX)
        .setShader(DEFAULT_FRAGMENT_SHADER, ShaderStage::FRAGMENT);
    pipelines[0] = builder.build();

    builder.setId(PR_PIPELINE_DEFAULT_UI);
    builder.setShader(DEFAULT_UI_VERTEX_SHADER, ShaderStage::VERTEX)
        .setShader(DEFAULT_UI_FRAGMENT_SHADER, ShaderStage::FRAGMENT);

    pipelines[1] = builder.build();

    return pipelines;

    // TODO: Implement default texture in the RenderingManager
    // drawManager_->setTexture(PR_DEFAULT_TEXTURE,
    // OpenGLTexture(DEFAULT_TEXTURE));
};  // namespace Presto

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

void OpenGLRenderer::render(MeshRegistrationData& data) {
    OpenGLMeshContext* context{contexts_.find(data.context_id)};
    if (context == nullptr) {
        PR_CORE_ERROR(
            "Unable to find OpenGL context for registered data with context id "
            "{}. "
            "Skipping this draw.",
            data.context_id);
        return;
    }

    // If the uniforms have been updated or the material has been updated,
    // update the values
    if (this->isDirty()) {
        updateUniforms();
    }

    context->vao.bind();

    glDrawElements(context->draw_mode, context->index_count, GL_UNSIGNED_INT,
                   nullptr);
};

void OpenGLRenderer::updateUniforms() {
    // TODO: Move these binds somewhere else
    globalUniformBuffer_->write(
        std::span<std::byte>(reinterpret_cast<std::byte*>(&globalUniforms_),
                             sizeof(GlobalUniforms)));
    globalUniformBuffer_->bind(0);

    objectUniformBuffer_->write(
        std::span<std::byte>(reinterpret_cast<std::byte*>(&objectUniforms_),
                             sizeof(ObjectUniforms)));
    objectUniformBuffer_->bind(1);

    setDirty(false);
};

Allocated<PipelineBuilder> OpenGLRenderer::getPipelineBuilder() {
    return Allocated<PipelineBuilder>{new OpenGLPipelineBuilder{}};
}

Allocated<UniformBuffer> OpenGLRenderer::createUniformBuffer(
    Presto::size_t size) {
    auto buffer{std::make_unique<OpenGLUniformBuffer>(size)};
    return buffer;
};

Allocated<Buffer> OpenGLRenderer::createBuffer(Buffer::BufferType type,
                                               Presto::size_t size) {
    return std::make_unique<OpenGLBuffer>(type, size);
};

Allocated<TextureFactory> OpenGLRenderer::getTextureFactory() {
    auto factory{std::make_unique<OpenGLTextureFactory>()};
    return factory;
};

bool OpenGLRenderer::createMeshContext(MeshRegistrationData& registration,
                                       const PipelineStructure& structure) {
    if (registration.context_id != PR_UNREGISTERED) {
        PR_CORE_WARN(
            "Attempted to create a mesh context for registration {}, which "
            "already has a mesh context of id {}. Skipping this request.",
            registration.render_manager_id, registration.context_id);
        return false;
    }

    OpenGLVAO new_vao{};

    Allocated<OpenGLMeshContext> new_context{new OpenGLMeshContext{
        .vao = {dynamic_cast<OpenGLBuffer*>(registration.vertices.get()),
                dynamic_cast<OpenGLBuffer*>(registration.indices.get()),
                structure},
        .index_count = static_cast<GLsizei>(registration.indices->size()),
    }};

    auto allocation{contexts_.alloc(std::move(new_context))};

    if (allocation.second == nullptr) {
        PR_ERROR("Unable to create Mesh Context in OpenGLRenderer.");
        return false;
    }

    registration.context_id = allocation.first;

    return true;
};
}  // namespace Presto
