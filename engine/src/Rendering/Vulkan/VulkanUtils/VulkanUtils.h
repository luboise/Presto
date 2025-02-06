
#pragma once

#include <vulkan/vulkan_core.h>

namespace Presto::VulkanUtils {

//////////////////////////////
// Initialisation Functions //
//////////////////////////////

VkInstance getNewVulkanInstance();
void initialiseVulkanExtensions();

//////////////////////////
// Validation Functions //
//////////////////////////

std::vector<const char*> getRequiredExtensions();
bool checkDeviceExtensionSupport(const VkPhysicalDevice& device);

bool checkValidationLayerSupport();

void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT& createInfo);

// Ping Vulkan for creation function
VkResult CreateDebugUtilsMessengerEXT(
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
    const VkAllocationCallbacks* pAllocator,
    VkDebugUtilsMessengerEXT* pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance instance,
                                   VkDebugUtilsMessengerEXT debugMessenger,
                                   const VkAllocationCallbacks* pAllocator);

// Debug callback function for Vulkan
VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

void setupDebugMessenger(VkInstance, VkDebugUtilsMessengerEXT*);

VkApplicationInfo makeApplicationInfo();

uint32_t findMemoryType(VkPhysicalDevice, uint32_t, VkMemoryPropertyFlags);

template <typename IN_T>
auto MapHandles(const std::vector<IN_T>& in) {
    using OUT_T = decltype(std::declval<IN_T>().handle());

    std::vector<OUT_T> out(in.size());

    std::transform(in.begin(), in.end(), out.begin(),
                   [](const auto& item) { return item.handle(); });
    return std::move(out);
}

template <typename IN_T>
auto MapHandles(const std::vector<IN_T*>& in) {
    using OUT_T = decltype(std::declval<IN_T*>()->handle());

    std::vector<OUT_T> out(in.size());

    std::transform(in.begin(), in.end(), out.begin(),
                   [](const auto& item) { return item->handle(); });
    return std::move(out);
}

}  // namespace Presto::VulkanUtils
