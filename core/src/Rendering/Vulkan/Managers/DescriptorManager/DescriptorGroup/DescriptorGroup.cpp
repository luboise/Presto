#include "DescriptorGroup.h"
#include <memory>

#include "PrestoCore/Core.h"

#include "Rendering/Vulkan/Abstractions/DescriptorPool.h"
#include "Rendering/Vulkan/Managers/BufferManager/BufferManager.h"
#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanUtils.h"

namespace PrestoCore {

    DescriptorGroup::DescriptorGroup(
        DescriptorPool& pool, BufferManager& buffer_manager,
        const std::vector<DescriptorSetLayout*>& layouts, uint32_t count)
        : _pool(&pool) {
        PR_CORE_ASSERT(count > 0,
                       "Must allocate at least one descriptor set when calling "
                       "DescriptorSetLayout::allocate()");

        SetLayoutList layout_handles = VulkanUtils::MapHandles(layouts);

        this->_setLayouts = layout_handles;

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = _pool->handle();

        const auto& device = _pool->getDevice();

        for (int i = 0; i < count; i++) {
            auto allocation = _pool->allocate(layout_handles);

            PR_CORE_TRACE(
                "Allocated {} descriptor sets from descriptor pool {}", count,
                fmt::ptr(&pool));

            DescriptorFrameSet dfs{allocation, {}};
            for (auto* set : allocation) {
                auto* buf = buffer_manager.createUniformBuffer(200);
                dfs.uniform_buffers.push_back(std::unique_ptr<Buffer>(buf));

                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = buf->handle();
                bufferInfo.offset = 0;
                bufferInfo.range = sizeof(ShaderMatrices);

                VkWriteDescriptorSet descriptorWrite{};
                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = set;
                descriptorWrite.descriptorType =
                    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrite.descriptorCount = 1;
                descriptorWrite.pBufferInfo = &bufferInfo;

                // Unused
                descriptorWrite.dstBinding = 0;
                descriptorWrite.dstArrayElement = 0;

                descriptorWrite.pImageInfo = nullptr;
                descriptorWrite.pTexelBufferView = nullptr;

                vkUpdateDescriptorSets(device.handle(), 1, &descriptorWrite, 0,
                                       nullptr);
            }

            _frameSets.push_back(std::move(dfs));
        }
    };

    const DescriptorFrameSet& DescriptorGroup::getFrameSet(
        uint32_t index) const {
        PR_CORE_ASSERT(index < _frameSets.size(),
                       "Invalid index into descriptor group. Expected [0, {}], "
                       "received {}.",
                       _frameSets.size() - 1, index);

        return _frameSets[index];
    };

}  // namespace PrestoCore
