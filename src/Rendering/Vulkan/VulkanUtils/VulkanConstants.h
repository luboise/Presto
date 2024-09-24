#pragma once

#include <vulkan/vulkan_core.h>

namespace Presto {

#ifdef NDEBUG
    constexpr bool enableValidationLayers = false;
#else
    constexpr bool enableValidationLayers = true;
#endif

    constexpr uint32_t ALLOCATED_VERTICES = 512;
    constexpr uint32_t ALLOCATED_INDICES = 2048;

    const std::vector<const char*> extensionList = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};
    constexpr int MAX_FRAMES_IN_FLIGHT = 2;

    constexpr auto MAIN_LAYER_NAME = "Main";

}  // namespace Presto
