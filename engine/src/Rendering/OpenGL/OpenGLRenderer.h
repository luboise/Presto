#pragma once

#include "Presto/Rendering/RenderTypes.h"
#include "Rendering/OpenGL/OpenGLDrawManager/OpenGLDrawManager.h"

#include "Presto/Rendering/Renderer.h"

namespace Presto {
    class GLFWAppWindow;
    /*
       The renderer.
       */

    class OpenGLRenderer final : public Renderer {
       public:
        explicit OpenGLRenderer(GLFWAppWindow* window);

        // Deleted functions
        OpenGLRenderer(const OpenGLRenderer&) = delete;
        OpenGLRenderer(OpenGLRenderer&&) = delete;
        OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
        OpenGLRenderer& operator=(OpenGLRenderer&&) = delete;

        // Destructor
        ~OpenGLRenderer() override;

        renderer_mesh_id_t loadMesh(MeshData data) override;
        void unloadMesh(renderer_mesh_id_t id) override;

        /*
renderer_material_id_t loadMaterial(MaterialData) override;
void unloadMaterial(renderer_material_id_t id) override;
        */

        renderer_texture_id_t loadTexture(Presto::Image image) override;
        void unloadTexture(renderer_texture_id_t id) override;

        void bindMaterial(const MaterialData& data) override;
        void unbindMaterial() override;

        void render(renderer_mesh_id_t meshId) override;

        void nextFrame() override;

       private:
        OpenGLMaterial* currentMaterial_{nullptr};

        void updateUniforms();

        void draw(const OpenGLMeshInfo& meshInfo);

        void onFrameBufferResized() override;
        std::unique_ptr<OpenGLDrawManager> drawManager_;

        void setupDebugLogging();

        static void debugCallback(GLenum source, GLenum type, GLuint id,
                                  GLenum severity, GLsizei length,
                                  const GLchar* message, const void* userParam);
    };

}  // namespace Presto
