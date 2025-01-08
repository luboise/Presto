#include "../CommandBuffer.h"

#include <vulkan/vulkan_core.h>

#include "Presto/Core.h"

#include "Rendering/Vulkan/Managers/BufferManager/BufferManager.h"
#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanTypes.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanUtils.h"

#include "Rendering/Vulkan/Abstractions/CommandPool.h"
#include "Rendering/Vulkan/Abstractions/RenderPass.h"

namespace Presto {
    CommandBuffer::CommandBuffer(CommandPool& pool)
        : Abstraction(), _pool(pool) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = _pool.handle();
        // Primary commandbuffer (can submit directly)
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)1;

        const VulkanDevice& device = pool.getLogicalDevice();

        if (vkAllocateCommandBuffers(device.handle(), &allocInfo, &_handle) !=
            VK_SUCCESS) {
            throw std::runtime_error("Unable to create command buffers.");
        }
    };

    CommandBuffer::~CommandBuffer() {
        auto* device = _pool.getLogicalDevice().handle();
        vkFreeCommandBuffers(device, _pool.handle(), 1, &_handle);
    }

    void CommandBuffer::startRecording(uint flags) {
        // Begin recording command buffer
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;

        if (vkBeginCommandBuffer(_handle, &beginInfo) != VK_SUCCESS) {
            PR_CORE_CRITICAL("Unable to begin recording to command buffer.");
            throw std::runtime_error("Unable to write to command buffer.");
        };
    }

    void CommandBuffer::stopRecording() { vkEndCommandBuffer(_handle); }

    void CommandBuffer::bind(Pipeline& pipeline) {
        this->_currentBoundPipeline = &pipeline;

        // Bind the command buffer to the pipeline
        vkCmdBindPipeline(_handle, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline.handle());
    }

    void CommandBuffer::bind(const DescriptorFrameSet& dfs) {
        PR_CORE_ASSERT(this->_currentBoundPipeline != nullptr,
                       "Unable to bind descriptor sets without a bound "
                       "pipeline in command buffer {}.",
                       fmt::ptr(_handle));

        const auto& sets = dfs.sets;

        const auto FIRST_SET = 0;
        const auto DYNAMIC_OFFSETS = 0;

        vkCmdBindDescriptorSets(_handle, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                _currentBoundPipeline->layout()->handle(),
                                FIRST_SET, static_cast<uint32_t>(sets.size()),
                                sets.data(), DYNAMIC_OFFSETS, nullptr);
    }

    void CommandBuffer::bind(const VulkanDrawInfo& draw_info) {
        this->bind(*draw_info.pipeline);
        this->bind(*draw_info.vertex_buffer, draw_info.vertex_offset);
        this->bind(*draw_info.index_buffer, draw_info.index_offset);
    }

    void CommandBuffer::bind(const BufferList& buffers,
                             std::vector<unsigned long> offsets) {
        PR_CORE_ASSERT(buffers.size() > 0,
                       "The number of buffers received to bind is 0. You must "
                       "bind at least 1 buffer when calling bind.")

        PR_CORE_ASSERT(
            buffers.size() == offsets.size(),
            "The number of buffers and offsets received is mismatched.");

        const auto handles = VulkanUtils::MapHandles(buffers);

        if (buffers[0]->getType() == BUFFER_TYPE::VERTEX) {
            vkCmdBindVertexBuffers(_handle, 0, buffers.size(), handles.data(),
                                   offsets.data());
        } else if (buffers[0]->getType() == BUFFER_TYPE::INDEX) {
            for (int i = 0; i < buffers.size(); i++) {
                vkCmdBindIndexBuffer(_handle, handles[i], offsets[i],
                                     VK_INDEX_TYPE_UINT32);
            }
        } else {
            PR_CORE_ASSERT(true == false,
                           "Invalid buffer type was bound. Buffer address: {}",
                           fmt::ptr(buffers[0]));
        }
    }

    void CommandBuffer::beginPass(const RenderPass& pass,
                                  VkFramebuffer framebuffer) {
        auto begin_info = pass.getBeginInfo(framebuffer);

        // INLINE -> execute from primary buffers
        vkCmdBeginRenderPass(_handle, &begin_info, VK_SUBPASS_CONTENTS_INLINE);

        // TODO: Move these to separate functions in CommandBuffer
        // Set viewport
        auto extent = pass.getSwapchain().getExtent();

        VkViewport viewport{};
        viewport.x = 0.0F;
        viewport.y = 0.0F;
        viewport.width = static_cast<float>(extent.width);
        viewport.height = static_cast<float>(extent.height);
        viewport.minDepth = 0.0F;
        viewport.maxDepth = 1.0F;
        vkCmdSetViewport(_handle, 0, 1, &viewport);

        // Set scissor (cuts viewport)
        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = extent;
        vkCmdSetScissor(_handle, 0, 1, &scissor);
    };
}  // namespace Presto
