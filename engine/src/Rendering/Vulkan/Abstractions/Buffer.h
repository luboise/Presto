#pragma once

#include <vulkan/vulkan_core.h>
#include "Abstraction.h"

namespace Presto {
class VulkanDevice;
class CommandPool;

#define PrestoVulkanBufferCreateArgs                                    \
    VkDeviceSize, VkBufferUsageFlags, VkMemoryPropertyFlags, VkBuffer&, \
        VkDeviceMemory&

struct Buffer : public Abstraction<VkBuffer> {
    enum BUFFER_TYPE { STAGING, VERTEX, INDEX, UNIFORM };

    Buffer(const VulkanDevice&, BUFFER_TYPE, VkDeviceSize,
           bool create_mapping = false);

    [[nodiscard]] uint32_t getSize() const { return this->_bufferSize; }

    void write(uint32_t size, const void* data, uint32_t offset);

    void write(const uint32_t size, const void* data) {
        this->write(size, data, 0);
    };

    void* createMapping(uint32_t offset, uint32_t size);

    // Overloads
    void* createMapping() { return this->createMapping(0, this->_bufferSize); };
    void* createMapping(const uint32_t offset) {
        return this->createMapping(offset, this->_bufferSize - offset);
    };

    [[nodiscard]] void* getMapping() const { return this->_memoryMapping; };
    void destroyMapping();

    [[nodiscard]] BUFFER_TYPE getType() const { return _type; }

    ~Buffer();

    explicit operator VkBuffer() const { return this->handle(); };
    [[nodiscard]] VkBuffer handle() const { return this->_handle; }

    static void copyInto(CommandPool&, const Buffer&, Buffer&, uint32_t size);
    static void copyInto(CommandPool& command_pool, const Buffer& from,
                         Buffer& to) {
        copyInto(
            command_pool, from, to,
            std::clamp(from.getSize(), static_cast<uint32_t>(0), to.getSize()));
    };

   private:
    static void createBufferVerbose(const VulkanDevice&, VkDeviceSize,
                                    VkBufferUsageFlags, VkMemoryPropertyFlags,
                                    VkBuffer&, VkDeviceMemory&);

    const VulkanDevice& _device;

    BUFFER_TYPE _type;
    VkDeviceSize _bufferSize;
    VkDeviceMemory _memory = nullptr;

    // Pointer to the mapping
    void* _memoryMapping = nullptr;
};
}  // namespace Presto
