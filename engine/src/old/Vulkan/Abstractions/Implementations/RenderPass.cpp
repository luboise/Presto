#include "../RenderPass.h"

#include "Rendering/Vulkan/Abstractions/Swapchain.h"

namespace Presto {
VkAttachmentDescription RenderPass::BasicColourAttachment = {
    // 1 Sample, no multisampling for color attachment
    .samples = VK_SAMPLE_COUNT_1_BIT,

    // Clear to black before drawing
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    // Keep value in memory after computing
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
    // Not using stencil buffer
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,

    // Finalise image for presenting after render pass
    .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
};

RenderPass::RenderPass(const Swapchain& swapchain,
                       std::vector<VkAttachmentDescription>& attachments)
    : Abstraction(), _swapchain(swapchain) {
    // This 0 is LOCATION 0
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    // Use the implicit (previous) subpass as a dependency
    VkSubpassDependency subpassDependency{};
    subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    // Index 0 is the only defined subpass
    subpassDependency.dstSubpass = 0;

    // THIS SUBPASS waits on color attachment stage before rendering to
    // framebuffer
    subpassDependency.srcStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.srcAccessMask = 0;

    // OTHER (future) SUBPASSES wait on color attachment before
    // transitioning to next subpass
    subpassDependency.dstStageMask =
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    // Create the render pass info object
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

    renderPassInfo.attachmentCount = attachments.size();
    renderPassInfo.pAttachments = attachments.data();

    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    // Defines subpass transition behaviour
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &subpassDependency;

    auto format = _swapchain.getImageFormat();
    for (auto& attachment : attachments) {
        attachment.format = format;
    }

    if (vkCreateRenderPass(_swapchain.getDevice()->handle(), &renderPassInfo,
                           nullptr, &_handle) != VK_SUCCESS) {
        throw std::runtime_error("Unable to create render pass.");
    }
}

VkRenderPassBeginInfo RenderPass::getBeginInfo(
    VkFramebuffer framebuffer) const {
    VkRenderPassBeginInfo begin_info{};
    begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    begin_info.renderPass = _handle;
    begin_info.framebuffer = framebuffer;

    begin_info.renderArea.offset = {0, 0};
    begin_info.renderArea.extent = _swapchain.getExtent();

    VkClearValue clearColor = {{{0.0F, 0.0F, 0.0F, 1.0F}}};
    begin_info.clearValueCount = 1;
    begin_info.pClearValues = &clearColor;

    return begin_info;
}

const Swapchain& RenderPass::getSwapchain() const { return _swapchain; }
}  // namespace Presto
