#include <vulkan/vulkan_core.h>

namespace Presto {
struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return this->graphicsFamily.has_value() && presentFamily.has_value();
    };

    static const QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice&,
                                                      const VkSurfaceKHR&);
};
}  // namespace Presto
