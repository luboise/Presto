#pragma once

#include "Presto/Core/Constants.h"
#include "Presto/Rendering/MaterialData.h"
#include "RenderTypes.h"  // IWYU pragma: export

#include "Presto/Rendering/MeshData.h"

namespace Presto {
    class GLFWAppWindow;

    class Renderer {
       public:
        Renderer() = default;

        void setWindow(GLFWAppWindow* window) { this->_glfwWindow = window; }

        void setExtents(VisualExtents extents) { extents_ = extents; };
        [[nodiscard]] VisualExtents getExtents() const { return extents_; }

        virtual renderer_mesh_id_t loadMesh(MeshData data) = 0;
        virtual void unloadMesh(renderer_mesh_id_t id) = 0;

        virtual renderer_material_id_t loadMaterial(MaterialData material) = 0;
        virtual void unloadMaterial(renderer_material_id_t id) = 0;

        virtual renderer_texture_id_t loadTexture(Presto::Image image) = 0;
        virtual void unloadTexture(renderer_texture_id_t id) = 0;

        virtual void render(renderer_mesh_id_t meshId,
                            renderer_texture_id_t materialId,
                            glm::mat4 transform) = 0;

        virtual void nextFrame() = 0;

        void setViewMatrix(const glm::mat4& newViewMatrix) {
            renderViewMatrix_ = newViewMatrix;
        }

        void setViewMatrix(glm::mat4&& newViewMatrix) {
            renderViewMatrix_ = newViewMatrix;
        }

        void framebufferResized() { this->_framebufferResized = true; }
        virtual void onFrameBufferResized() = 0;

        virtual ~Renderer() = default;

       protected:
        GLFWAppWindow* _glfwWindow{nullptr};
        glm::mat4 renderViewMatrix_{};
        bool _framebufferResized{false};

       private:
        VisualExtents extents_{};
    };
}  // namespace Presto
