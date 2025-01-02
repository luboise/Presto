#pragma once

#include <map>

#include "PrestoCore/Rendering/Camera.h"
#include "PrestoCore/Rendering/RenderTypes.h"

namespace Presto {
    class GLFWAppWindow;

    enum SHADER_MODULE_TYPE { VERTEX, FRAGMENT };

    class PRESTO_API Renderer {
       public:
        Renderer() = default;

        void setWindow(GLFWAppWindow* window) { this->_glfwWindow = window; }

        virtual void addToRenderPool(draw_info_key) = 0;
        virtual void render(draw_info_key) = 0;
        virtual void nextFrame() = 0;

        void setCamera(Camera& newCam) { _renderCamera = &newCam; }

        void framebufferResized() { this->_framebufferResized = true; }
        virtual void onFrameBufferResized() {}

        virtual ~Renderer() = default;

       protected:
        GLFWAppWindow* _glfwWindow = nullptr;

        bool _framebufferResized = false;
        Camera* _renderCamera = nullptr;
    };
}  // namespace Presto
