#include "VulkanRenderer.h"

#include <GLFW/glfw3.h>

namespace Presto {
    PR_RESULT VulkanRenderer::drawFrame() {
        VkResult res;
        // Wait for previous frame (1 fence, wait all fences) then reset fence
        // to unsignaled
        vkWaitForFences(_logicalDevice, 1, &_inFlightFences[_currentFrame],
                        VK_TRUE, UINT64_MAX);

        // Check for framebuffer resize
        // if (_framebufferResized) {
        //     _framebufferResized = false;
        //     this->recreateSwapChain();
        //     return PR_SUCCESS;
        // }

        // Acquire image from swap chain to draw into
        uint32_t imageIndex;
        res = vkAcquireNextImageKHR(_logicalDevice, _swapchain, UINT64_MAX,
                                    _imageAvailableSemaphores[_currentFrame],
                                    VK_NULL_HANDLE, &imageIndex);

        if (res == VK_ERROR_OUT_OF_DATE_KHR) {
            _framebufferResized = false;
            this->recreateSwapChain();
            return PR_SUCCESS;
        } else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
            PR_CORE_ERROR("Failed to acquire swap chain image.");
            return PR_FAILURE;
        }

        // Only reset fence if image is acquired
        vkResetFences(_logicalDevice, 1, &_inFlightFences[_currentFrame]);

        // Reset, then record command buffer which draws our scene into the
        // image
        vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);
        this->recordCommandBuffer(_commandBuffers[_currentFrame], imageIndex);

        // Which semaphores to wait on to draw
        VkSemaphore waitSemaphores[] = {
            _imageAvailableSemaphores[_currentFrame]};
        VkPipelineStageFlags waitStages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        // Signalled by submit queue
        VkSemaphore signalSemaphores[] = {
            _renderFinishedSemaphores[_currentFrame]};

        // Submit instructions (waits for waitSemaphores)
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        // Which pipeline stages wait (want colour attachment to wait until
        // buffer available)
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &_commandBuffers[_currentFrame];

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

        res = vkQueueSubmit(_graphicsQueue, 1, &submitInfo,
                            _inFlightFences[_currentFrame]);
        if (res != VK_SUCCESS) {
            PR_CORE_CRITICAL("Failed to submit command buffer to GPU.");
            return PR_FAILURE;
        }

        // Check that the image was presented properly
        res = vkQueuePresentKHR(_presentQueue, &presentInfo);
        if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR ||
            _framebufferResized) {
            // If the swapchain was out of date when we went to present it,
            // recreate it and try again next frame
            _framebufferResized = false;
            this->recreateSwapChain();
        } else if (res != VK_SUCCESS) {
            PR_CORE_CRITICAL("Failed to present new frame from queue.");
            return PR_FAILURE;
        }

        _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

        return PR_SUCCESS;
    }

    void VulkanRenderer::recreateSwapChain() {
        int width, height = 0;

        // TODO: Move this logic out
        glfwGetFramebufferSize(this->_glfwWindow, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(this->_glfwWindow, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(_logicalDevice);

        cleanupSwapChain();

        createSwapChain();
        createImageViews();
        createFrameBuffers();
    }

    void VulkanRenderer::cleanupSwapChain() {
        for (size_t i = 0; i < _swapchainFramebuffers.size(); i++) {
            vkDestroyFramebuffer(_logicalDevice,
                                 this->_swapchainFramebuffers[i], nullptr);
        }

        for (size_t i = 0; i < _swapchainImageViews.size(); i++) {
            vkDestroyImageView(_logicalDevice, _swapchainImageViews[i],
                               nullptr);
        }

        vkDestroySwapchainKHR(_logicalDevice, _swapchain, nullptr);
    }

}  // namespace Presto