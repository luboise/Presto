#include "../VulkanBuffer.h"

#include <vulkan/vulkan_core.h>
#include <stdexcept>

#include <Rendering/Vulkan/Abstractions/CommandPool.h>
#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanUtils.h"

#include "Rendering/Vulkan/MemoryMapping.h"

#include "Rendering/Vulkan/MemoryAllocation.h"

namespace Presto {

VulkanBuffer::VulkanBuffer(const VulkanDevice& device, const BufferType& type,
                           const VkDeviceSize size, const bool create_mapping)
    : Buffer(type, size), device_(device.handle()) {
    // TODO: Re-implement staging buffer
    /*
case STAGING:
createBufferVerbose(device, size_, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                    handle_, memory_);
break;
            */

    VkBufferUsageFlags usage_flags{0};
    VkMemoryPropertyFlags memory_flags{0};

    switch (type) {
        case BufferType::VERTEX:
            usage_flags = VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            memory_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            break;
        case BufferType::INDEX:
            usage_flags = VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                          VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            memory_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
            break;
        case BufferType::UNIFORM:
            usage_flags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            memory_flags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
            break;
        default:
            throw std::invalid_argument("Unable to handle Buffer type.");
    }

    // Create buffer
    VkBufferCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size = size;
    createInfo.usage = usage_flags;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    auto* logicalDevice = device.handle();
    if (vkCreateBuffer(logicalDevice, &createInfo, nullptr, &buffer_) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to create vertex buffer.");
    }

    VkMemoryRequirements reqs;
    vkGetBufferMemoryRequirements(logicalDevice, buffer_, &reqs);

    memory_ = std::make_unique<MemoryAllocation>(
        logicalDevice, device.getPhysicalDevice(), memory_flags, reqs);
}

void VulkanBuffer::write(buffer_write_t data, Presto::size_t offset) {
    /**
    PR_CORE_ASSERT(mapping_ != nullptr,
                   "A buffer can't be written to without a memory mapping "
                   "(see VulkanBuffer::getMapping).");
**/

    if (offset >= data.size()) {
        PR_ERROR(
            "Offset {} is out of range for input data of size {}. Skipping "
            "this write.",
            offset, data.size());

        return;
    }

    Presto::size_t write_size{getWriteSize(data.size(), offset)};
    memory_->withMapping([&data, write_size](auto& span) {
        std::memcpy(span.data(), data.data(), write_size);
    });
};

VulkanBuffer::~VulkanBuffer() {
    memory_->unmap();

    vkDestroyBuffer(device_, this->handle_, nullptr);
    vkFreeMemory(device_, this->memory_, nullptr);
}

void VulkanBuffer::createBufferVerbose(const VulkanDevice& device,
                                       VkDeviceSize size,
                                       VkBufferUsageFlags usage_flags,
                                       VkMemoryPropertyFlags memory_flags,
                                       VkBuffer& bufferOut,
                                       VkDeviceMemory& memoryOut) {
    // Allocate memory on GPU

    // Get memory requirements of device to allocate memory
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logicalDevice, bufferOut, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;

    allocInfo.memoryTypeIndex = VulkanUtils::findMemoryType(
        device.getPhysicalDevice(), memRequirements.memoryTypeBits,
        memory_flags);

    if (vkAllocateMemory(logicalDevice, &allocInfo, nullptr, &memoryOut) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate vertex buffer memory.");
    }

    // Bind allocate memory to buffer (at offset 0, otherwise must
    // be divisible by memRequirements.alignment)
    vkBindBufferMemory(logicalDevice, bufferOut, memoryOut, 0);
};

MemoryMapping VulkanBuffer::createMapping(uint32_t offset, uint32_t size) {
    return MemoryMapping(offset, size);

    const uint32_t allocation_size = this->size_ - offset;

    // Check for valid mapping region
    PR_CORE_ASSERT(
        allocation_size <= size_,
        "A memory mapping can't be made past the boundaries of a buffer.");

    // Check there isn't already a mapping
    PR_CORE_ASSERT(mapping_ == nullptr,
                   "A memory mapping can't be created if there already is "
                   "one.\nBuffer handle: {}\nBuffer memory handle: {}, "
                   "Existing map pointer: {}'",
                   fmt::ptr(handle_), fmt::ptr(memory_), fmt::ptr(mapping_));

    vkMapMemory(device_, memory_, offset, allocation_size, 0, &mapping_);
    return mapping_;
};

void VulkanBuffer::destroyMapping() {
    PR_CORE_ASSERT(mapping_ != nullptr,
                   "A null memory mapping can't be destroyed.");

    vkUnmapMemory(_device.handle(), memory_);
    mapping_ = nullptr;
}

void VulkanBuffer::write(const uint32_t data_size, const void* data,
                         const uint32_t offset) {
    /**
    PR_CORE_ASSERT(mapping_ != nullptr,
                   "A buffer can't be written to without a memory mapping "
                   "(see VulkanBuffer::getMapping).");
**/

    auto* mapping = this->createMapping(offset, data_size);

    memcpy(mapping, data, data_size);

    this->destroyMapping();
};

void VulkanBuffer::copyInto(CommandPool& command_pool, const Buffer& src,
                            Buffer& dst, const uint32_t size) {
    PR_CORE_ASSERT(
        src._device.handle() == dst._device.handle(),
        "A buffer can not be copied across different logical devices.");

    const auto& device = src._device;

    /**
// void VulkanRenderer::copyBuffer(VkBuffer src, VkBuffer dst,
// VkDeviceSize copySize)
VkCommandBufferAllocateInfo allocInfo{};
allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
allocInfo.commandPool = command_pool;
allocInfo.commandBufferCount = 1;

    VkCommandBuffer copyCommandBuffer;
    vkAllocateCommandBuffers(device, &allocInfo, &copyCommandBuffer);
    **/

    auto* temp_buffer = command_pool.createCommandBuffer();
    temp_buffer->startRecording(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;

    vkCmdCopyBuffer(temp_buffer->handle(), static_cast<VkBuffer>(src),
                    static_cast<VkBuffer>(dst), 1, &copyRegion);
    temp_buffer->stopRecording();

    device.submitToGraphicsQueue(temp_buffer->handle());

    command_pool.destroyCommandBuffer(temp_buffer);
}
}  // namespace Presto
