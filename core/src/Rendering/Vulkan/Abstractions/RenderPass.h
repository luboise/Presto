#pragma once

#include <vulkan/vulkan_core.h>

#include "Abstraction.h"
#include "Swapchain.h"

namespace PrestoCore {
    class RenderPass : public Abstraction<VkRenderPass> {
       public:
        static VkAttachmentDescription BasicColourAttachment;

        RenderPass(const Swapchain& swapchain,
                   std::vector<VkAttachmentDescription>& attachments);

        ~RenderPass();

        [[nodiscard]] VkRenderPassBeginInfo getBeginInfo(
            VkFramebuffer framebuffer) const;

        [[nodiscard]] const Swapchain& getSwapchain() const;

       private:
        const Swapchain& _swapchain;
    };
}  // namespace PrestoCore
