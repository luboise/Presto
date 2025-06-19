#include "MemoryMapping.h"

#include <vulkan/vulkan.h>

namespace Presto {

MemoryMapping::MemoryMapping(Presto::uint32_t offset, Presto::uint32_t size) {
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

MemoryMapping::operator std::span<std::byte>() const {}

MemoryMapping::~MemoryMapping() {
    PR_CORE_ASSERT(mapping_ != nullptr,
                   "A null memory mapping can't be destroyed.");

    if (mapping_ != VK_NULL_HANDLE) {
        vkUnmapMemory(device_, memory_);
        mapping_ = nullptr;
    }
}

}  // namespace Presto
