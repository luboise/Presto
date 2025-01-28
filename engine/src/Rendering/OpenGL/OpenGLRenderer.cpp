#include "OpenGLRenderer.h"

#include "OpenGLDrawManager/OpenGLDrawManager.h"

#include "Presto/Core/Constants.h"
#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Rendering/Renderer.h"
#include "Rendering/OpenGL/OpenGLPipeline.h"
#include "Rendering/Utils/RenderingUtils.h"

#include "OpenGLDrawManager/DefaultShaders.h"

#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <stdexcept>

#include "Presto/Runtime/GLFWAppWindow.h"

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

        ShaderPtr default_shader = std::make_shared<OpenGLShader>();
        default_shader->setShader(DEFAULT_VERTEX_SHADER, ShaderStage::VERTEX)
            .setShader(DEFAULT_FRAGMENT_SHADER, ShaderStage::FRAGMENT)
            .linkShaderProgram();

        drawManager_->setMaterial(PR_PIPELINE_DEFAULT_3D,
                                  OpenGLPipeline{default_shader});

        /*
                constexpr auto POS_VECTOR_SIZE = 3;
                constexpr auto TRIANGLE_POINT_COUNT = POS_VECTOR_SIZE * 3;

                PointType triangle[] = {0.0f, 0.5f,  0.0f,  0.5f, -0.5f,
                                        0.0f, -0.5f, -0.5f, 0.0f};
                                                                        */
    }

    void OpenGLRenderer::nextFrame() {
        glfwSwapBuffers(
            static_cast<GLFWwindow*>(_glfwWindow->getWindowHandle()));
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
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW,
                              0, nullptr, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
                              GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr,
                              GL_FALSE);

        // Allow error, warning, and deprecated behavior messages
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_ERROR, GL_DONT_CARE,
                              0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR,
                              GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR,
                              GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE,
                              GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PORTABILITY,
                              GL_DONT_CARE, 0, nullptr, GL_TRUE);

        glDebugMessageCallback(debugCallback, nullptr);
    };

    void OpenGLRenderer::onFrameBufferResized() {
        auto extents{_glfwWindow->getFramebufferSize()};
        this->setExtents(extents);

        glViewport(0, 0, static_cast<GLsizei>(extents.width),
                   static_cast<GLsizei>(extents.height));
    }

    renderer_mesh_id_t OpenGLRenderer::loadMesh(MeshData data) {
        return this->drawManager_->addMesh(data);
    };

    void OpenGLRenderer::unloadMesh(renderer_mesh_id_t id) {
        this->drawManager_->removeMesh(id);
    };

    renderer_texture_id_t OpenGLRenderer::loadTexture(Presto::Image image) {
        return drawManager_->addTexture(image);
    };

    void OpenGLRenderer::unloadTexture(renderer_texture_id_t id) {
        drawManager_->removeTexture(id);
    };

    void OpenGLRenderer::render(renderer_mesh_id_t meshId) {
        auto* mesh{drawManager_->getMeshInfo(meshId)};
        // auto* albedo{drawManager_->getTexture(imageId)};

        if (mesh == nullptr) {
            PR_CORE_ERROR(
                "OpenGL render function received nullptr mesh data of ID {}.\n"
                "Skipping this mesh.",
                meshId);
            return;
        }

        PR_CORE_ASSERT(currentMaterial_ != nullptr,
                       "OpenGL is unable to render with no material set.");

        this->draw(*mesh);
    };

    void OpenGLRenderer::draw(const OpenGLMeshInfo& meshInfo) {
        // If the uniforms have been updated or the material has been updated,
        // update the values
        if (this->isDirty()) {
            updateUniforms();
        }

        glBindVertexArray(meshInfo.vao);
        glDrawElements(meshInfo.draw_mode, meshInfo.index_count,
                       GL_UNSIGNED_INT, nullptr);

        /*
PR_CORE_ASSERT(
    draw_data.mat_props.texture.isLoaded(),
    "Draw data is used without being loaded in OpenGL Renderer.");
        */
    };

    void OpenGLRenderer::bindMaterial(const MaterialData& data) {
        auto id{data.materialType};

        OpenGLPipeline* material{drawManager_->getMaterial(id)};

        PR_ASSERT(material != nullptr,
                  "Unable to bind non-existant material from id {}.", id);

        if (material == currentMaterial_) {
            PR_CORE_TRACE(
                "Redundant material binding has occurred for material with ID "
                "{}. Skipping this binding.",
                id);
            return;
        }

        OpenGLTexture* diffuse{drawManager_->getTexture(data.diffuseTexture)};
        PR_ASSERT(diffuse != nullptr,
                  "Unable to bind material with null diffuse texture.");

        material->setDiffuse(diffuse);

        material->bind();

        currentMaterial_ = material;

        setDirty();
    };

    void OpenGLRenderer::unbindMaterial() {
        currentMaterial_->unbind();
        currentMaterial_ = nullptr;
    };

    void OpenGLRenderer::updateUniforms() {
        PR_CORE_ASSERT(currentMaterial_ != nullptr,
                       "Unable to setup uniforms on a null material.");

        ShaderPtr shader{currentMaterial_->getShader()};

        shader->setUniform("view", globalUniforms_.view);
        shader->setUniform("projection", globalUniforms_.projection);
        // TODO: Make this a one-off function call
        shader->setUniform("sampler1", std::int8_t(0));

        shader->setUniform("transform", objectUniforms_.transform);

        setDirty(false);
    };
}  // namespace Presto
