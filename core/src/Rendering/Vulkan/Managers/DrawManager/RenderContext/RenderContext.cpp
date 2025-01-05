
#include "RenderContext.h"

namespace PrestoCore {
    RenderContext::RenderContext(const Swapchain& swapchain) {
        std::vector<VkAttachmentDescription> attachments = {
            RenderPass::BasicColourAttachment};

        auto* render_pass = new RenderPass(swapchain, attachments);
        this->_renderPass = render_pass;
    }
}  // namespace PrestoCore
