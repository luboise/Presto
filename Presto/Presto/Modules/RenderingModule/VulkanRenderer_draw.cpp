#include "VulkanRenderer.h"
namespace Presto {
    PR_RESULT VulkanRenderer::drawFrame() {
        // Wait for previous frame (1 fence, wait all fences) then reset fence
        // to unsignaled
        vkWaitForFences(_logicalDevice, 1, &_inFlightFence, VK_TRUE,
                        UINT64_MAX);
        vkResetFences(_logicalDevice, 1, &_inFlightFence);

        // Acquire image from swap chain to draw into
        uint32_t imageIndex;
        vkAcquireNextImageKHR(_logicalDevice, _swapchain, UINT64_MAX,
                              _imageAvailableSemaphore, VK_NULL_HANDLE,
                              &imageIndex);

        // Reset, then record command buffer which draws our scene into the
        // image
        vkResetCommandBuffer(_commandBuffer, 0);
        this->recordCommandBuffer(_commandBuffer, imageIndex);

        // Which semaphores to wait on to draw
        VkSemaphore waitSemaphores[] = {_imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        // Signalled by submit queue
        VkSemaphore signalSemaphores[] = {_renderFinishedSemaphore};

        // Submit instructions (waits for waitSemaphores)
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        // Which pipeline stages wait (want colour attachment to wait until
        // buffer available)
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &_commandBuffer;

        // Triggers these signals when finished
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        // Presentation instructions (waits for signalSemaphores)
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        // Which swapchains to draw to, and which image for each chain
        presentInfo.swapchainCount = 1;
        VkSwapchainKHR swapchains[] = {_swapchain};

        // presentInfo.pSwapchains = &_swapchain;
        presentInfo.pSwapchains = swapchains;
        presentInfo.pImageIndices = &imageIndex;

        VkResult res;

        res = vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFence);
        if (res != VK_SUCCESS) {
            PR_CORE_CRITICAL("Failed to submit command buffer to GPU.");
            return PR_FAILURE;
        }

        res = vkQueuePresentKHR(_presentQueue, &presentInfo);
        if (res != VK_SUCCESS) {
            PR_CORE_CRITICAL("Failed to present new frame from queue.");
            return PR_FAILURE;
        }
        return PR_SUCCESS;

        // Present swap chain image
    }

}  // namespace Presto