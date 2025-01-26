#include "OpenGLRenderer.h"

#include "OpenGLDrawManager/OpenGLDrawManager.h"

#include "Presto/Rendering/RenderTypes.h"
#include "Presto/Rendering/Renderer.h"
#include "Rendering/Utils/RenderingUtils.h"

#include <glm/gtc/type_ptr.hpp>

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

    void GLAPIENTRY OpenGLRenderer::debugCallback(GLenum source, GLenum type,
                                                  GLuint id, GLenum severity,
                                                  GLsizei length,
                                                  const GLchar* message,
                                                  const void* userParam) {
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

    void OpenGLRenderer::unloadMaterial(renderer_material_id_t id) {};

    renderer_material_id_t OpenGLRenderer::loadMaterial(MaterialData material) {
    };

    renderer_texture_id_t OpenGLRenderer::loadTexture(Presto::Image image) {
        return drawManager_->addTexture(image);
    };

    void OpenGLRenderer::unloadTexture(renderer_texture_id_t id) {};

    void OpenGLRenderer::render(renderer_mesh_id_t meshId,
                                renderer_texture_id_t imageId,
                                glm::mat4 transform) {
        auto* mesh{drawManager_->getMeshInfo(meshId)};
        auto* albedo{drawManager_->getTexture(imageId)};

        // TODO: Implement drawing using the transform
        if (mesh == nullptr || albedo == nullptr) {
            PR_CORE_ERROR(
                "getRenderable() returned a nullptr for renderable. "
                "Skipping drawing.");
            return;
        }

        this->draw(*mesh, *albedo, transform);
    };

    void OpenGLRenderer::draw(const OpenGLMeshInfo& meshInfo,
                              const OpenGLTexture& albedoTexture,
                              glm::mat4 transform) {
        auto slot = 0;
        albedoTexture.bind(slot);

        /*
PR_CORE_ASSERT(
    draw_data.mat_props.texture.isLoaded(),
    "Draw data is used without being loaded in OpenGL Renderer.");
        */

        glUseProgram(meshInfo.shader_program);

        // TODO: Make this a one-off function call
        {
            GLint sampler{
                glGetUniformLocation(meshInfo.shader_program, "sampler1")};
            PR_CORE_ASSERT(sampler != -1,
                           "Sampler1 is not found in the shader program.");
            // Set sampler1 to be using texture slot 0
            glUniform1i(sampler, slot);
        }

        ShaderMatrices mats{};

        constexpr glm::float32 FOV_Y = 90;

        mats.view = renderViewMatrix_;

        auto framebufferSize{_glfwWindow->getFramebufferSize()};

        mats.projection = RenderingUtils::getProjectionMatrix(
            FOV_Y, framebufferSize.width, framebufferSize.height);
        mats.transform = transform;

        GLint view{glGetUniformLocation(meshInfo.shader_program, "view")};
        glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(mats.view));

        GLint projection{
            glGetUniformLocation(meshInfo.shader_program, "projection")};
        glUniformMatrix4fv(projection, 1, GL_FALSE,
                           glm::value_ptr(mats.projection));

        GLint transformLoc{
            glGetUniformLocation(meshInfo.shader_program, "transform")};
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE,
                           glm::value_ptr(mats.transform));

        glBindVertexArray(meshInfo.vao);
        glDrawElements(meshInfo.draw_mode, meshInfo.index_count,
                       GL_UNSIGNED_INT, nullptr);
    };
}  // namespace Presto
