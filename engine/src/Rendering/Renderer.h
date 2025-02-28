#pragma once

#include <map>
#include "Presto/Utils/LazyCalculator.h"

#include "Presto/Rendering/RenderTypes.h"  // IWYU pragma: export

#include "Rendering/Buffer.h"

#include "Rendering/MeshRegistrationData.h"
#include "TextureFactory.h"

namespace Presto {
class GLFWAppWindow;
class CameraComponent;
class Image;
class UniformLayout;
struct ImportedMesh;

class Pipeline;
class PipelineBuilder;

class UniformBuffer;

class Renderer : protected LazyCalculator {
   public:
    static Allocated<Renderer> create(RENDER_LIBRARY lib,
                                      GLFWAppWindow* window);

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
    virtual Allocated<TextureFactory> getTextureFactory() = 0;
    // virtual Allocated<Texture> createTexture(Presto::Image image) = 0;

    virtual Allocated<Buffer> createBuffer(Buffer::BufferType type,
                                           Presto::size_t size) = 0;
    virtual Allocated<UniformBuffer> createUniformBuffer(
        Presto::size_t size) = 0;

    using AllocatedPipelineList =
        std::vector<std::pair<pipeline_id_t, Allocated<Pipeline>>>;
    virtual AllocatedPipelineList createDefaultPipelines() = 0;

    virtual bool createMeshContext(MeshRegistrationData&) = 0;
    virtual void render(MeshRegistrationData&) = 0;

    void setWindow(GLFWAppWindow* window) { this->_glfwWindow = window; }

    void setExtents(VisualExtents extents) { extents_ = extents; };
    [[nodiscard]] VisualExtents getExtents() const { return extents_; }

    virtual void nextFrame() = 0;

    void setCameraData(CameraComponent& camera);
    void setCameraData(GlobalUniforms&&);

    void setObjectData(ObjectUniforms&&);

    void framebufferResized() { this->_framebufferResized = true; }
    virtual void onFrameBufferResized() = 0;

    /* [[nodiscard]] virtual std::vector<PipelineStructure>
       getPipelineStructures() const = 0;
                 */

   protected:
    Renderer() = default;

    GLFWAppWindow* _glfwWindow{nullptr};

    GlobalUniforms globalUniforms_{};
    ObjectUniforms objectUniforms_{};

    bool _framebufferResized{false};

   private:
    VisualExtents extents_{};
};
}  // namespace Presto
