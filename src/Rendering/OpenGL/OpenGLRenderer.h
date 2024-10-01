#pragma once

#include <GL/glew.h>

#include "Presto/Rendering/Renderer.h"
#include "RenderableManager/RenderableManager.h"

namespace Presto {
    class GLFWAppWindow;
    /*
       The renderer.
       */

    class PRESTO_API OpenGLRenderer final : public Renderer {
       public:
        explicit OpenGLRenderer(GLFWAppWindow* window);
        ~OpenGLRenderer() override;

        void addToRenderPool(draw_info_key renderable) override;
        void render(draw_info_key key) override;
        void nextFrame() override;

       private:
        void draw(const OpenGlRenderable&);
        void onFrameBufferResized() override {}
        RenderableManager _renderableManager;
        OpenGlRenderable _renderable;
    };

}  // namespace Presto
