#pragma once

#include <GL/glew.h>

#include "Presto/Rendering/Renderer.h"
#include "Rendering/OpenGL/OpenGLDrawManager/OpenGLDrawManager.h"

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

        render_data_id_t registerRenderGroup(const RenderGroup& data) override;
        render_data_id_t registerRenderGroup(RenderGroup&& data) override;

        void unregisterMesh(render_data_id_t id) override;
        void render(render_data_id_t id, glm::mat4 transform) override;
        void nextFrame() override;

       private:
        void draw(const OpenGLDrawBatch&, const glm::mat4& transform);
        void onFrameBufferResized() override {}
        std::unique_ptr<OpenGLDrawManager> drawManager_;
    };

}  // namespace Presto
