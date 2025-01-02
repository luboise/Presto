#include "DrawManager.h"

#include <vulkan/vulkan_core.h>

#include <exception>

#include "PrestoCore/Core.h"
#include "PrestoCore/Log.h"

#include "Rendering/Vulkan/VulkanUtils/VulkanConstants.h"

#include "Rendering/Vulkan/Abstractions/CommandBuffer.h"
#include "Rendering/Vulkan/Abstractions/CommandPool.h"
#include "Rendering/Vulkan/Abstractions/Pipeline.h"
#include "Rendering/Vulkan/Abstractions/Swapchain.h"

namespace Presto {
    using VulkanDrawContext = DrawManager::VulkanDrawContext;

    DrawManager::DrawManager(Swapchain& swapchain, const uint32_t count)
        : _swapchain(swapchain) {
        auto* device = swapchain.getDevice();
        this->_commandPool = device->createCommandPool();

        _commandPool = device->createCommandPool();

        for (int i = 0; i < count; i++) {
            _syncSets.push_back(device->createSyncSet());
            _commandBuffers.push_back(_commandPool->createCommandBuffer());
        }

        this->_drawContext = getDrawContext();
    };

    DrawManager::~DrawManager() {
        delete _commandPool;

        this->destroyFramebuffers();
    }

    VulkanDrawContext DrawManager::getDrawContext() {
        return {_commandBuffers[_currentFrame], _syncSets[_currentFrame]};
    };

    void DrawManager::destroyFramebuffers() {
        for (size_t i = 0; i < _framebuffers.size(); i++) {
            vkDestroyFramebuffer(_swapchain.getDevice()->handle(),
                                 this->_framebuffers[i], nullptr);
        }
    }

    void DrawManager::reloadSwapchain() {
        vkDeviceWaitIdle(_swapchain.getDevice()->handle());
        this->destroyFramebuffers();
        this->_swapchain.reload();
        this->createFramebuffers(_swapchain,
                                 _currentRenderContext->getRenderPass());
    };

    PR_RESULT DrawManager::enableDrawing() {
        auto* device = _swapchain.getDevice()->handle();

        auto* fence = _drawContext.sync_set->frame_is_in_flight;
        PR_CORE_ASSERT(_drawContext.sync_set != nullptr,
                       "Unable to enable drawing with a nullptr sync_set "
                       "attribute on _drawContext.");

        vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX);
        // Check for framebuffer resize
        // if (_reloadSwapchainOnNextFrame) {
        //     _reloadSwapchainOnNextFrame = false;
        //     this->recreateSwapChain();
        //     return PR_SUCCESS;
        // }

        // Acquire image from swap chain to draw into
        VkResult res = _swapchain.getNextImage(
            _currentImageIndex, _drawContext.sync_set->image_is_available);

        if (res == VK_ERROR_OUT_OF_DATE_KHR) {
            _reloadSwapchainOnNextFrame = false;
            this->reloadSwapchain();
            return PR_FAILURE;
        }

        if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) {
            PR_CORE_ERROR("Failed to acquire swap chain image.");
            return PR_FAILURE;
        }

        // Only reset fence if image is acquired
        vkResetFences(device, 1, &_drawContext.sync_set->frame_is_in_flight);

        CommandBuffer& cmd = *_drawContext.command_buffer;

        // Reset, then record command buffer which draws our scene into the
        // image
        cmd.reset();

        try {
            cmd.startRecording();
        } catch (std::exception& e) {
            // TODO: Handle errors in a more robust way. At the moment, just
            // throw them.

            PR_CORE_ERROR(e.what());
            throw;
        }

        // Viewport and scissor are also set in beginPass
        cmd.beginPass(_currentRenderContext->getRenderPass(),
                      _framebuffers[_currentImageIndex]);

        return PR_SUCCESS;
    }

    void DrawManager::draw(const VulkanDrawInfo& info) {
        auto& command_buffer = *_drawContext.command_buffer;

        if (!_startedDrawing) {
            // Clear and enable writing on the command buffer
            this->enableDrawing();
            _startedDrawing = true;
        }

        // Bind the pipeline, vertex buffers, index buffer
        command_buffer.bind(info);

        // Bind the descriptor sets needed to draw
        command_buffer.bind(
            info.pipeline->getDescriptors().getFrameSet(_currentFrame));

        constexpr auto INSTANCES = 1;

        // Bind the vertex buffer for the operation
        vkCmdDrawIndexed(command_buffer.handle(),
                         static_cast<uint32_t>(info.index_count), INSTANCES,
                         info.index_offset, info.vertex_offset, 0);
    }

    void DrawManager::goToNextFrame() {
        if (!_startedDrawing) {
            this->advanceFrame();
            return;
        }

        this->disableDrawing();
        this->submitCommands();
    }

    void DrawManager::disableDrawing() {
        auto* cmd = _drawContext.command_buffer->handle();
        vkCmdEndRenderPass(cmd);

        if (vkEndCommandBuffer(cmd) != VK_SUCCESS) {
            PR_CORE_CRITICAL("Failed to record command buffer!");
            throw std::runtime_error("Failed to record command buffer!");
        }
    }

    void DrawManager::submitCommands() {
        // Update uniform buffers
        // auto time = glfwGetTime();
        // auto angle = glm::vec3(0, 0, 0);

        VkPipelineStageFlags waitStages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        // Submit instructions (waits for waitSemaphores)
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &_drawContext.sync_set->image_is_available;
        // Which pipeline stages wait (want colour attachment to wait until
        // buffer available)
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;

        const VkCommandBuffer cmd = _drawContext.command_buffer->handle();
        submitInfo.pCommandBuffers = &cmd;

        // Triggers these signals when finished
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores =
            &_drawContext.sync_set->render_is_finished;

        // Presentation instructions (waits for signalSemaphores)
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores =
            &_drawContext.sync_set->render_is_finished;

        // Which swapchains to draw to, and which image for each chain
        presentInfo.swapchainCount = 1;

        VkSwapchainKHR swapchains[] = {static_cast<VkSwapchainKHR>(_swapchain)};
        uint32_t imageIndices[] = {_currentImageIndex};

        // presentInfo.pSwapchains = &_swapchain;
        presentInfo.pSwapchains = swapchains;
        presentInfo.pImageIndices = imageIndices;

        auto* device = _swapchain.getDevice();

        if (vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo,
                          _drawContext.sync_set->frame_is_in_flight) !=
            VK_SUCCESS) {
            PR_CORE_CRITICAL("Failed to submit command buffer to GPU.");
        };

        // Check that the image was presented properly
        VkResult res =
            vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);

        if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR ||
            _reloadSwapchainOnNextFrame) {
            // If the swapchain was out of date when we went to present it,
            // recreate it and try again next frame
            _reloadSwapchainOnNextFrame = false;
            this->reloadSwapchain();
        } else if (res != VK_SUCCESS) {
            PR_CORE_CRITICAL("Failed to present new frame from queue.");
        }

        _currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
        _startedDrawing = false;
    }

    RenderContext* DrawManager::addLayer(const RenderCtxKey& layer_name) {
        PR_CORE_ASSERT(!_renderContexts.contains(layer_name),
                       "Attempted to add a second layer with name {}. This is "
                       "strictly not allowed.",
                       layer_name);

        auto* ctx = new RenderContext(_swapchain);
        this->_renderContexts[layer_name] = ctx;

        // If this is the first layer created
        if (this->_renderContexts.size() == 1) {
            this->setActiveLayer(layer_name);
            this->createFramebuffers(_swapchain,
                                     _currentRenderContext->getRenderPass());
        }

        return ctx;
    };

    void DrawManager::createFramebuffers(const Swapchain& swapchain,
                                         const RenderPass& pass) {
        const auto extent = swapchain.getExtent();
        std::vector<VkImageView> imageViews = swapchain.getImageViews();

        // Get framebuffers from image view (?)
        this->_framebuffers.resize(imageViews.size());

        // TODO: Check if moving this out of the loop  a difference
        VkFramebufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass = pass.handle();
        createInfo.attachmentCount = 1;
        createInfo.width = extent.width;
        createInfo.height = extent.height;
        createInfo.layers = 1;

        for (size_t i = 0; i < imageViews.size(); i++) {
            VkImageView attachments[] = {imageViews[i]};
            createInfo.pAttachments = attachments;

            if (vkCreateFramebuffer(swapchain.getDevice()->handle(),
                                    &createInfo, nullptr,
                                    &_framebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("Unable to create framebuffer.");
            }
        }
    }
}  // namespace Presto
