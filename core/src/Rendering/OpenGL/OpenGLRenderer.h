#pragma once

#include <GL/glew.h>

#include "PrestoCore/Rendering/Renderer.h"
#include "Rendering/OpenGL/OpenGLDrawManager/OpenGLDrawManager.h"

namespace Presto {
    class GLFWAppWindow;
    /*
       The renderer.
       */

    class PRESTO_API OpenGLRenderer final : public Renderer {
       public:
        explicit OpenGLRenderer(GLFWAppWindow* window);

        // Deleted functions
        OpenGLRenderer(const OpenGLRenderer&) = delete;
        OpenGLRenderer(OpenGLRenderer&&) = delete;
        OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
        OpenGLRenderer& operator=(OpenGLRenderer&&) = delete;

        // Destructor
        ~OpenGLRenderer() override;

        render_data_id_t registerMesh(const RenderData& data) override;
        render_data_id_t registerMesh(RenderData&& data) override;

        void unregisterMesh(render_data_id_t id) override;
        void render(render_data_id_t id, glm::vec4 transform) override;
        void nextFrame() override;

       private:
        void draw(const OpenGLDrawInfo&);
        void onFrameBufferResized() override {}
        OpenGLDrawManager* drawManager_;
    };

}  // namespace Presto
