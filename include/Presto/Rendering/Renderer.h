#pragma once

#include <map>

#include "Presto/Components/Renderable.h"

#include "Presto/Rendering/Camera.h"
#include "Presto/Rendering/RenderTypes.h"

namespace Presto {
    class GLFWAppWindow;

    using draw_info_key = Renderable*;
    using DrawInfoMap = std::map<draw_info_key, DrawInfo>;

    enum SHADER_MODULE_TYPE { VERTEX, FRAGMENT };

    class PRESTO_API Renderer {
       public:
        Renderer() = default;

        enum RENDER_LIBRARY { VULKAN, OPENGL, DIRECTX };
        void setWindow(GLFWAppWindow* window) { this->_glfwWindow = window; }

        virtual void addToRenderPool(draw_info_key) = 0;
        virtual void draw(draw_info_key) = 0;
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
