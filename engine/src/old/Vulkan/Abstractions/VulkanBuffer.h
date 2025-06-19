#pragma once

#include <vulkan/vulkan_core.h>

#include "Rendering/Buffer.h"

namespace Presto {

class VulkanDevice;
class CommandPool;
class MemoryMapping;
class MemoryAllocation;

/*
struct OpenGLBufferArgs {
    ByteArray bytes;
    std::size_t vertex_count;
    AttributeSet attributes;
};

class OpenGLBuffer final : public Buffer {
   public:
    OpenGLBuffer(BufferType type, Presto::size_t size);
    OpenGLBuffer(BufferType type, ByteArray& data);

    ~OpenGLBuffer() override;


   private:
    HandleType buffer_{};
    Presto::int32_t openGlBufferType_;
};
*/

struct VulkanBuffer final : public Buffer {
    using HandleType = VkBuffer;

    enum BUFFER_TYPE { STAGING, VERTEX, INDEX, UNIFORM };

    VulkanBuffer(const VulkanDevice&, const BufferType&, VkDeviceSize,
                 bool create_mapping = false);
    ~VulkanBuffer() override;

    void bind() override;
    [[nodiscard]] uint32_t getSize() const { return this->size_; }

    void write(buffer_write_t data, Presto::size_t offset = 0) override;

    /*
void write(uint32_t size, const void* data, uint32_t offset);

void write(const uint32_t size, const void* data) {
    this->write(size, data, 0);
};
    */

    MemoryMapping createMapping(uint32_t offset, uint32_t size);

    // Overloads
    void* createMapping() { return this->createMapping(0, this->size_); };
    void* createMapping(const uint32_t offset) {
        return this->createMapping(offset, this->size_ - offset);
    };

    [[nodiscard]] void* getMapping() const { return this->mapping_; };
    void destroyMapping();

    [[nodiscard]] BUFFER_TYPE getType() const { return type_; }

    explicit operator VkBuffer() const { return this->handle(); };
    [[nodiscard]] VkBuffer handle() const { return this->handle_; }

    static void copyInto(CommandPool&, const VulkanBuffer&, VulkanBuffer&,
                         uint32_t size);
    static void copyInto(CommandPool& command_pool, const VulkanBuffer& from,
                         VulkanBuffer& to) {
        copyInto(
            command_pool, from, to,
            std::clamp(from.getSize(), static_cast<uint32_t>(0), to.getSize()));
    };

   private:
    static void createBufferVerbose(const VulkanDevice&, VkDeviceSize,
                                    VkBufferUsageFlags, VkMemoryPropertyFlags,
                                    VkBuffer&, VkDeviceMemory&);

    VkDevice device_;

    HandleType buffer_{};

    VkDeviceSize size_{};

    Allocated<MemoryAllocation> memory_ = nullptr;

    // Pointer to the mapping
    void* mapping_ = nullptr;
};
}  // namespace Presto
