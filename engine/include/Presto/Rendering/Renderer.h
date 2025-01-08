#pragma once

#include "Presto/Core/Constants.h"
#include "RenderData.h"
#include "RenderTypes.h"  // IWYU pragma: export

namespace Presto {
    class GLFWAppWindow;

    enum SHADER_MODULE_TYPE { VERTEX, FRAGMENT };

    using render_data_id_t = PR_NUMERIC_ID;
    constexpr PR_NUMERIC_ID UNREGISTERED_RENDER_DATA_ID = -1;

    class Renderer {
       public:
        Renderer() = default;

        void setWindow(GLFWAppWindow* window) { this->_glfwWindow = window; }

        virtual render_data_id_t registerMesh(RenderData&&) = 0;
        virtual render_data_id_t registerMesh(const RenderData&) = 0;

        virtual void unregisterMesh(render_data_id_t id) = 0;
        virtual void render(render_data_id_t id, glm::mat4 transform) = 0;
        virtual void nextFrame() = 0;

        void setViewMatrix(const glm::mat4& newViewMatrix) {
            renderViewMatrix_ = newViewMatrix;
        }

        void setViewMatrix(glm::mat4&& newViewMatrix) {
            renderViewMatrix_ = newViewMatrix;
        }

        void framebufferResized() { this->_framebufferResized = true; }
        virtual void onFrameBufferResized() {}

        virtual ~Renderer() = default;

       protected:
        GLFWAppWindow* _glfwWindow = nullptr;
        glm::mat4 renderViewMatrix_{};
        bool _framebufferResized = false;
    };
}  // namespace Presto
