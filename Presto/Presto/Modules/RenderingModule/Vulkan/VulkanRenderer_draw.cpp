#include "VulkanRenderer.h"

#include <GLFW/glfw3.h>

#include <glm/gtc/constants.hpp>

namespace Presto {
    glm::vec3 getRandomRed(const double maxNonRed) {
        auto vec = glm::vec3(1.0f, ((rand() % 101) / 100.0) * maxNonRed,
                             ((rand() % 101) / 100.0) * maxNonRed);
        return vec;
    }

    glm::vec3 getRandomRed() { return getRandomRed(0.05); }

    std::vector<VulkanVertex> makeHeartSlow() {
        std::vector<VulkanVertex> heart;

        VulkanVertex startingPoint = {{0.0f, 0.5f, 0.0f}, getRandomRed()};
        VulkanVertex middlePoint = {{.0f, -.25f, .0f}, getRandomRed()};
        std::vector<VulkanVertex> leftHalf = {
            {{-0.5f, -0.15f, 0.0f}, getRandomRed()},
            {{-0.25f, -0.5f, 0.0f}, getRandomRed()},
        };

        std::vector<VulkanVertex> rightHalf;
        for (auto it = leftHalf.rbegin(); it != leftHalf.rend(); it++) {
            const auto& vertex = *it;
            auto vertexCopy = vertex;
            vertexCopy.pos.x *= -1;
            vertexCopy.color = getRandomRed();
            rightHalf.push_back(vertexCopy);
        }

        heart.push_back(startingPoint);
        for (const auto& v : leftHalf) {
            heart.push_back(v);
        };

        heart.push_back(middlePoint);

        for (auto it = leftHalf.rbegin(); it != leftHalf.rend(); it++) {
            VulkanVertex v = *it;
            v.pos.x *= -1;
            heart.push_back(v);
        }

        return heart;
    }

    // Curve modified from https://mathworld.wolfram.com/HeartCurve.html,
    // subtracted 2.5 and then divided by 15 to roughly scale the range
    // to [-1, 1]
    std::vector<VulkanVertex> makeHeart(uint32_t points) {
        // Symmetric shape, so need even number of points
        if (points % 2 == 1) points++;

        std::vector<VulkanVertex> heart(points);

        const float width = 1;
        const float height = width;

        const double denom = glm::two_pi<double>() / (points);
        double t = 0;
        for (auto i = 0; i < points; i++) {
            t = i * denom;

            float x = (width / 2.0f) * glm::pow(glm::sin(t), 3);
            // Normalised height
            float y = (13 * glm::cos(t) - 2 * glm::cos(3 * t) -
                       glm::cos(4 * t) - 5 * glm::cos(2 * t));
            y += 2.5;
            y /= 15;
            // Scale to desired height
            y *= (height / 2);

            heart[i] = VulkanVertex{{x, -y, 0.0f}, getRandomRed()};
        }

        return heart;
    };

    const std::vector<uint16_t> makeIndices(uint16_t vertexCount) {
        std::vector<uint16_t> indices(vertexCount);
        for (auto i = 0; i < vertexCount; i++) indices[i] = i;
        return indices;
    };

    // Defined external vectors
    const std::vector<VulkanVertex> vertices = makeHeart(HEART_POINTS);
    const std::vector<uint16_t> indices = makeIndices(HEART_POINTS);

    PR_RESULT
    VulkanRenderer::drawFrame() {
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

        ShaderMatrices mats{};

        // Update uniform buffers
        auto time = glfwGetTime();
        auto angle = glm::vec3(0, 0, 0);

        // auto cameraPos = glm::vec3(0, -2, 2);
        auto cameraPos = glm::vec3(0, 0, 0.75);

        auto fovYDeg = 90;
        double scale =
            (glm::sin(time * glm::two_pi<double>() / 3) * 0.15) + 0.85;

        mats.modelView =
            vertices[0].getModelViewMatrix(cameraPos, angle, scale);
        mats.projection =
            vertices[0].getProjectionMatrix(fovYDeg, this->_swapchainExtent);

        memcpy(_uniformBuffersMapped[_currentFrame], &mats, sizeof(mats));
        // END PUTTING STUFF IN GPU BUFFER

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

    void VulkanRenderer::copyBuffer(VkBuffer src, VkBuffer dst,
                                    VkDeviceSize copySize) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = _commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer copyCommandBuffer;
        vkAllocateCommandBuffers(_logicalDevice, &allocInfo,
                                 &copyCommandBuffer);

        // Begin recording command buffer
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(copyCommandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = copySize;

        vkCmdCopyBuffer(copyCommandBuffer, src, dst, 1, &copyRegion);
        vkEndCommandBuffer(copyCommandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &copyCommandBuffer;

        // Wait for submission (can use a fence instead for multiple transfers)
        vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(_graphicsQueue);

        vkFreeCommandBuffers(_logicalDevice, _commandPool, 1,
                             &copyCommandBuffer);
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