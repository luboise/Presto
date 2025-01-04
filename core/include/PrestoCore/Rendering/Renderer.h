#pragma once

#include "Camera.h"
#include "RenderData.h"
#include "RenderTypes.h"

namespace Presto {
    class GLFWAppWindow;

    enum SHADER_MODULE_TYPE { VERTEX, FRAGMENT };

    using render_data_id_t = id_t;

    class PRESTO_API Renderer {
       public:
        Renderer() = default;

        void setWindow(GLFWAppWindow* window) { this->_glfwWindow = window; }

        virtual render_data_id_t registerMesh(RenderData&&) = 0;
        virtual render_data_id_t registerMesh(const RenderData&) = 0;

        virtual void unregisterMesh(render_data_id_t id) = 0;
        virtual void render(render_data_id_t id, glm::vec4 transform) = 0;
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
