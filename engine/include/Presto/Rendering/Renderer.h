#pragma once

#include "Presto/Mixins/LazyCalculator.h"

#include "Presto/Rendering/PipelineTypes.h"
#include "Presto/Rendering/Texture.h"
#include "RenderTypes.h"  // IWYU pragma: export
#include "Rendering/Buffer.h"

namespace Presto {
class GLFWAppWindow;
class CameraComponent;
class Image;
class MaterialStructure;
struct ImportedMesh;

class PipelineBuilder;

class UniformBuffer;

class Renderer : protected LazyCalculator {
   public:
    Renderer() = default;
    virtual ~Renderer() = default;

    /**
     * Returns a builder that can be used to create new graphics
     * pipelines. The builder is determined by the renderer implementation, and
     * is assumed to be able to create new graphics pipelines by its own means.
     *
     * @brief Returns a builder used to create new graphics pipelines.
     * @return The new PipelineBuilder object.
     */
    virtual Allocated<PipelineBuilder> getPipelineBuilder() = 0;

    virtual Allocated<Texture> createTexture(Presto::Image image) = 0;
    virtual Allocated<Buffer> createBuffer(Buffer::BufferType type,
                                           Presto::size_t size) = 0;

    virtual Allocated<UniformBuffer> createUniformBuffer(
        Presto::size_t size) = 0;

    virtual void unloadMesh(renderer_mesh_id_t id) = 0;

    virtual void bindMeshToPipeline(renderer_mesh_id_t, renderer_pipeline_id_t);

    virtual void usePipeline(renderer_pipeline_id_t) = 0;

    virtual void bindMaterial(const MaterialStructure&) = 0;
    virtual void unbindMaterial() = 0;

    void setWindow(GLFWAppWindow* window) { this->_glfwWindow = window; }

    void setExtents(VisualExtents extents) { extents_ = extents; };
    [[nodiscard]] VisualExtents getExtents() const { return extents_; }
    // TODO: Implement custom materials/shaders
    /*
virtual renderer_material_id_t loadMaterial(MaterialData material) = 0;
virtual void unloadMaterial(renderer_material_id_t id) = 0;
    */

    virtual void render(renderer_mesh_id_t meshId) = 0;

    virtual void nextFrame() = 0;

    void setCameraData(CameraComponent& camera);
    void setCameraData(GlobalUniforms&&);

    void setObjectData(ObjectUniforms&&);

    void framebufferResized() { this->_framebufferResized = true; }
    virtual void onFrameBufferResized() = 0;

    [[nodiscard]] virtual std::vector<PipelineStructure> getPipelineStructures()
        const = 0;

   protected:
    GLFWAppWindow* _glfwWindow{nullptr};

    GlobalUniforms globalUniforms_{};
    ObjectUniforms objectUniforms_{};

    bool _framebufferResized{false};

   private:
    VisualExtents extents_{};
};
}  // namespace Presto
