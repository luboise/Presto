#include "../Buffer.h"

#include <vulkan/vulkan_core.h>
#include <stdexcept>

#include <Rendering/Vulkan/Abstractions/CommandPool.h>
#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanUtils.h"

namespace Presto {

Buffer::Buffer(const VulkanDevice& device, const BUFFER_TYPE type,
               const VkDeviceSize size, const bool create_mapping)
    : Abstraction(), _device(device), _type(type), _bufferSize(size) {
    switch (type) {
        case STAGING:
            createBufferVerbose(device, _bufferSize,
                                VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                _handle, _memory);
            break;

        case VERTEX:
            createBufferVerbose(device, _bufferSize,
                                VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _handle,
                                _memory);
            break;

        case INDEX:
            createBufferVerbose(device, _bufferSize,
                                VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                    VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _handle,
                                _memory);
            break;

        case UNIFORM:

            // Uniform buffers

            createBufferVerbose(device, _bufferSize,
                                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                _handle, _memory);
            break;
        default:
            throw std::invalid_argument("Unable to handle Buffer type.");
    }

    if (create_mapping) {
        this->createMapping();
    }
}

Buffer::~Buffer() {
    auto* device_handle = _device.handle();

    if (_memoryMapping != nullptr && _memoryMapping != VK_NULL_HANDLE) {
        vkUnmapMemory(device_handle, _memory);
        _memoryMapping = nullptr;
    }

    vkDestroyBuffer(device_handle, _handle, nullptr);
    vkFreeMemory(device_handle, _memory, nullptr);
}

void Buffer::createBufferVerbose(const VulkanDevice& device, VkDeviceSize size,
                                 VkBufferUsageFlags usage_flags,
                                 VkMemoryPropertyFlags memory_flags,
                                 VkBuffer& bufferOut,
                                 VkDeviceMemory& memoryOut) {
    // Create buffer
    VkBufferCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    createInfo.size = size;
    createInfo.usage = usage_flags;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    auto* logicalDevice = device.handle();

    if (vkCreateBuffer(logicalDevice, &createInfo, nullptr, &bufferOut) !=
        VK_SUCCESS) {
        throw std::runtime_error("Failed to create vertex buffer.");
    }

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

void* Buffer::createMapping(const uint32_t offset, const uint32_t size) {
    const uint32_t allocation_size = this->_bufferSize - offset;

    // Check for valid mapping region
    PR_CORE_ASSERT(
        allocation_size <= _bufferSize,
        "A memory mapping can't be made past the boundaries of a buffer.");

    // Check there isn't already a mapping
    PR_CORE_ASSERT(_memoryMapping == nullptr,
                   "A memory mapping can't be created if there already is "
                   "one.\nBuffer handle: {}\nBuffer memory handle: {}, "
                   "Existing map pointer: {}'",
                   fmt::ptr(_handle), fmt::ptr(_memory),
                   fmt::ptr(_memoryMapping));

    vkMapMemory(_device.handle(), _memory, offset, allocation_size, 0,
                &_memoryMapping);
    return _memoryMapping;
};

void Buffer::destroyMapping() {
    PR_CORE_ASSERT(_memoryMapping != nullptr,
                   "A null memory mapping can't be destroyed.");

    vkUnmapMemory(_device.handle(), _memory);
    this->_memoryMapping = nullptr;
}

void Buffer::write(const uint32_t data_size, const void* data,
                   const uint32_t offset) {
    /**
    PR_CORE_ASSERT(_memoryMapping != nullptr,
                   "A buffer can't be written to without a memory mapping "
                   "(see Buffer::getMapping).");
**/

    auto* mapping = this->createMapping(offset, data_size);

    memcpy(mapping, data, data_size);

    this->destroyMapping();
};

void Buffer::copyInto(CommandPool& command_pool, const Buffer& src, Buffer& dst,
                      const uint32_t size) {
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
