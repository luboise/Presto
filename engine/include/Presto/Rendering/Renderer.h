#pragma once

#include "Presto/Mixins/LazyCalculator.h"

#include "RenderTypes.h"  // IWYU pragma: export

namespace Presto {
class GLFWAppWindow;
class CameraComponent;
class Image;
class MaterialStructure;
struct ImportedMesh;

class Renderer : protected LazyCalculator {
   public:
    Renderer() = default;

    virtual ~Renderer() = default;

    void setWindow(GLFWAppWindow* window) { this->_glfwWindow = window; }

    void setExtents(VisualExtents extents) { extents_ = extents; };
    [[nodiscard]] VisualExtents getExtents() const { return extents_; }

    virtual renderer_mesh_id_t loadMesh(const ImportedMesh&) = 0;
    virtual void unloadMesh(renderer_mesh_id_t id) = 0;

    virtual void bindMeshToPipeline(renderer_mesh_id_t, renderer_pipeline_id_t);

    // TODO: Implement custom materials/shaders
    /*
virtual renderer_material_id_t loadMaterial(MaterialData material) = 0;
virtual void unloadMaterial(renderer_material_id_t id) = 0;
    */

    virtual renderer_texture_id_t loadTexture(Presto::Image image) = 0;
    virtual void unloadTexture(renderer_texture_id_t id) = 0;

    virtual void bindMaterial(const MaterialStructure&) = 0;
    virtual void unbindMaterial() = 0;

    virtual void render(renderer_mesh_id_t meshId) = 0;

    virtual void nextFrame() = 0;

    void setCameraData(CameraComponent& camera);
    void setCameraData(GlobalUniforms&&);

    void setObjectData(ObjectUniforms&&);

    void framebufferResized() { this->_framebufferResized = true; }
    virtual void onFrameBufferResized() = 0;

   protected:
    GLFWAppWindow* _glfwWindow{nullptr};

    GlobalUniforms globalUniforms_{};
    ObjectUniforms objectUniforms_{};

    bool _framebufferResized{false};

   private:
    VisualExtents extents_{};
};
}  // namespace Presto
