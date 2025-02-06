#pragma once

#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/Abstractions/RenderPass.h"

namespace Presto {
class Pipeline;

class RenderContext {
   public:
    explicit RenderContext(const Swapchain&);

    [[nodiscard]] RenderPass& getRenderPass() const {
        PR_CORE_ASSERT(
            _renderPass != nullptr,
            "Attempted to get the render pass handle from RenderContext "
            "before it has been initialised. This shouldn't be possible. "
            "Check the render context constructor RenderContext()")
        return *_renderPass;
    };

    [[nodiscard]] Pipeline* getPipeline() const { return _pipeline; }

    void setPipeline(Pipeline* new_pipeline) { this->_pipeline = new_pipeline; }

   private:
    Pipeline* _pipeline;
    RenderPass* _renderPass;
};

}  // namespace Presto
