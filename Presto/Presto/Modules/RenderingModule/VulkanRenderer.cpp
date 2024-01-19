#include "VulkanRenderer.h"

#include <algorithm>
#include <limits>
#include <set>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace Presto {
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

    const std::vector<const char*> extensionList = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VulkanRenderer::VulkanRenderer(GLFWwindow* window) : _glfwWindow(window) {
        this->Init();
    }

    VulkanRenderer::~VulkanRenderer() {
        if (this->IsInitialised()) this->Shutdown();
    };

    void VulkanRenderer::Init() {
        PR_RESULT res;
        res = this->createInstance();
        if (res == PR_SUCCESS) {
            res = this->setupDebugMessenger();
        }
        if (res == PR_SUCCESS) {
            res = this->createSurface();
        }
        if (res == PR_SUCCESS) {
            res = this->pickPhysicalDevice();
        }
        if (res == PR_SUCCESS) {
            res = this->createLogicalDevice();
        }
        if (res == PR_SUCCESS) {
            res = this->createSwapChain();
        }
        this->_initialised = (res == PR_SUCCESS) ? true : false;
    }

    void VulkanRenderer::Shutdown() {
        if (enableValidationLayers) {
            this->DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger,
                                                nullptr);
            this->_debugMessenger = nullptr;
        }

        vkDestroySwapchainKHR(_logicalDevice, _swapchain, nullptr);

        vkDestroySurfaceKHR(_instance, _surface, nullptr);
        _surface = nullptr;

        vkDestroyDevice(_logicalDevice, nullptr);
        this->_logicalDevice = nullptr;

        vkDestroyInstance(_instance, nullptr);
        this->_instance = nullptr;

        this->_initialised = false;
    }

    void VulkanRenderer::initialiseVulkanExtensions() {
        // Create empty vector of Vulkan extensions
        uint32_t extensionCount = 0;
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                               extensions.data());

        // Log available extensions
        std::stringstream ss;
        ss << "Available extensions: ";
        for (const auto& extension : extensions) {
            ss << extension.extensionName << ", ";
        }
        PR_CORE_INFO(ss.str());
    }

    SwapChainSupportDetails VulkanRenderer::querySwapChainSupport(
        VkPhysicalDevice device) const {
        SwapChainSupportDetails details;

        // Read surface capabilities into VkSurfaceCapabilitiesKHR
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface,
                                                  &details.capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount,
                                             nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount,
                                                 details.formats.data());
        }

        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface,
                                                  &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device, _surface, &presentModeCount,
                details.presentModes.data());
        }

        return details;
    }

    bool VulkanRenderer::checkValidationLayerSupport() {
        // Get number of global layer properties
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        // Check that each validation layer is in the list of availablelayers
        for (const char* layerName : validationLayers) {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            if (!layerFound) {
                return false;
            }
        }
        return true;
    }

    bool VulkanRenderer::checkDeviceExtensionSupport(
        const VkPhysicalDevice& device) const {
        // Get list of available extensions
        uint32_t extensionCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                             nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                             availableExtensions.data());

        std::set<std::string> requiredExtensions(extensionList.begin(),
                                                 extensionList.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    VkSurfaceFormatKHR VulkanRenderer::chooseSwapSurfaceFormat(
        const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        PR_ASSERT(
            availableFormats.size() > 0,
            "Empty list of swap surfaces passed to chooseSwapSurfaceFormat.");

        // Look for SRGB with 8 bit colors
        for (const auto& formatKHR : availableFormats) {
            if (formatKHR.format == VK_FORMAT_B8G8R8A8_SRGB &&
                formatKHR.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR) {
                return formatKHR;
            }
        }

        // Backup is to return the first format
        return availableFormats[0];
    }

    PR_RESULT VulkanRenderer::createInstance() {
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            return PR_FAILURE;
        }

        // Instance info return object
        VkInstanceCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        auto extensions = this->getRequiredExtensions();
        createInfo.enabledExtensionCount =
            static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        this->initialiseVulkanExtensions();

        // Attach createInstance debugger if validation layers on
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (enableValidationLayers) {
            createInfo.enabledLayerCount =
                static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            this->populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext =
                (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        } else {
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        VkApplicationInfo appInfo = this->makeApplicationInfo();
        createInfo.pApplicationInfo = &appInfo;

        // Create and check instance
        VkResult result =
            vkCreateInstance(&createInfo, nullptr, &(this->_instance));

        PR_ASSERT(result == VK_SUCCESS, "Unable to create Vulkan instance.");

        PR_CORE_INFO("Created Vulkan instance.");

        PR_RESULT pr_result = (result == VK_SUCCESS) ? PR_SUCCESS : PR_FAILURE;
        return pr_result;
    }
    PR_RESULT VulkanRenderer::setupDebugMessenger() {
        if (!enableValidationLayers) return PR_SUCCESS;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        auto res = CreateDebugUtilsMessengerEXT(
            this->_instance, &createInfo, nullptr, &this->_debugMessenger);

        return (res == VK_SUCCESS) ? PR_SUCCESS : PR_FAILURE;
    }
    PR_RESULT VulkanRenderer::createSurface() {
        if (glfwCreateWindowSurface(_instance, this->_glfwWindow, nullptr,
                                    &_surface) != VK_SUCCESS) {
            PR_CORE_CRITICAL("Failed to create window surface.");
            return PR_FAILURE;
        } else {
            return PR_SUCCESS;
        }
    }
    PR_RESULT VulkanRenderer::createSwapChain() {
        SwapChainSupportDetails swapChainSupport =
            querySwapChainSupport(this->_physicalDevice);

        VkSurfaceFormatKHR surfaceFormat =
            chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode =
            chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D swapExtent = chooseSwapExtent(swapChainSupport.capabilities);

        // Want an extra image that we can render to while the first x are
        // performing operations
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if (swapChainSupport.capabilities.maxImageCount > 0)
            if (imageCount > swapChainSupport.capabilities.maxImageCount) {
                imageCount = swapChainSupport.capabilities.maxImageCount;
            }

        VkSwapchainCreateInfoKHR createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = this->_surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;

        createInfo.imageExtent = swapExtent;

        // Higher for stereoscopic 3D
        createInfo.imageArrayLayers = 1;

        // Render it straight (colour attachment). Could change this later to do
        // post processing
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(this->_physicalDevice);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
                                         indices.presentFamily.value()};

        // Draw on graphics queue, then submit to presentation queue

        // If different queues, use concurrent mode
        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }  // Otherwise use exclusive
        else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;      // Optional
            createInfo.pQueueFamilyIndices = nullptr;  // Optional
        }

        // No rotation transformations
        createInfo.preTransform =
            swapChainSupport.capabilities.currentTransform;

        // Ignore alpha channel between windows
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;

        // Ignore pixels that are obscured (eg. behind other windows)
        createInfo.clipped = VK_TRUE;

        // No old swapchain exists
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        auto res = vkCreateSwapchainKHR(this->_logicalDevice, &createInfo,
                                        nullptr, &(this->_swapchain));

        if (res != PR_SUCCESS) {
            PR_CORE_CRITICAL("Unable to create Vulkan swapchain.");
            return PR_FAILURE;
        } else
            return PR_SUCCESS;
    }

    bool VulkanRenderer::isDeviceSuitable(const VkPhysicalDevice& device) {
        auto indices = this->findQueueFamilies(device);
        bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            SwapChainSupportDetails swapChainSupport =
                querySwapChainSupport(device);
            swapChainAdequate = !swapChainSupport.formats.empty() &&
                                !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

    QueueFamilyIndices VulkanRenderer::findQueueFamilies(
        const VkPhysicalDevice& device) {
        QueueFamilyIndices indices;

        // Find queue family indices
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                                 nullptr);

        if (queueFamilyCount != 0) {
            std::vector<VkQueueFamilyProperties> queueFamilies(
                queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                                     queueFamilies.data());
            int queue_index = 0;
            for (const auto& queueFamily : queueFamilies) {
                // Check if family can do graphics
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    indices.graphicsFamily = queue_index;
                }

                // Check if queue family can present to the window
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, queue_index,
                                                     _surface, &presentSupport);
                if (presentSupport) {
                    indices.presentFamily = queue_index;
                }

                if (indices.isComplete()) break;

                queue_index++;
            }
        }

        return indices;
    }

    PR_RESULT VulkanRenderer::pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(this->_instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            PR_CORE_CRITICAL(
                "Unable to find a Vulkan compatible graphics "
                "device.");
            return PR_FAILURE;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(this->_instance, &deviceCount,
                                   devices.data());

        // Find first valid device
        for (const auto& device : devices) {
            if (VulkanRenderer::isDeviceSuitable(device)) {
                this->_physicalDevice = device;
                break;
            }
        }

        if (this->_physicalDevice == VK_NULL_HANDLE) {
            PR_CORE_CRITICAL("No suitable graphics device found.");
            return PR_FAILURE;
        }

        return PR_SUCCESS;
    }

    PR_RESULT VulkanRenderer::createLogicalDevice() {
        QueueFamilyIndices indices = findQueueFamilies(_physicalDevice);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {
            indices.graphicsFamily.value(), indices.presentFamily.value()};

        const float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        // Device Queue Props
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        queueCreateInfo.pQueuePriorities = &queuePriority;

        // Physical Device Features
        VkPhysicalDeviceFeatures deviceFeatures{};

        // Using these to create the logical device
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.queueCreateInfoCount =
            static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.enabledExtensionCount =
            static_cast<uint32_t>(extensionList.size());
        createInfo.ppEnabledExtensionNames = extensionList.data();

        // Handle validation layers
        if (enableValidationLayers) {
            createInfo.enabledLayerCount =
                static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(_physicalDevice, &createInfo, nullptr,
                           &_logicalDevice) != VK_SUCCESS) {
            PR_CORE_CRITICAL("Unable to create logical device.");
            return PR_FAILURE;
        }

        vkGetDeviceQueue(_logicalDevice, indices.graphicsFamily.value(), 0,
                         &_graphicsQueue);
        vkGetDeviceQueue(_logicalDevice, indices.presentFamily.value(), 0,
                         &_presentQueue);

        return PR_SUCCESS;
    }

    VkPresentModeKHR VulkanRenderer::chooseSwapPresentMode(
        const std::vector<VkPresentModeKHR>& availablePresentModes) {
        // Check if mailbox is available
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }

        // Default (always available)
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanRenderer::chooseSwapExtent(
        const VkSurfaceCapabilitiesKHR& capabilities) {
        // Equal to (unsigned)-1 when we should select best ourself
        if (capabilities.currentExtent.width !=
            std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(this->_glfwWindow, &width, &height);

            // Get size of framebuffer (size of window)
            VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                                       static_cast<uint32_t>(height)};

            // Clamp this to the valid range supported by the device
            actualExtent.width = std::clamp(actualExtent.width,
                                            capabilities.minImageExtent.width,
                                            capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(
                actualExtent.height, capabilities.minImageExtent.height,
                capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    VkResult VulkanRenderer::CreateDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
        const VkAllocationCallbacks* pAllocator,
        VkDebugUtilsMessengerEXT* pDebugMessenger) {
        // Get placeholder of function pointer for
        // vkCreateDebugUtilsMessengerEXT
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            // Call the function if it exists
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void VulkanRenderer::DestroyDebugUtilsMessengerEXT(
        VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks* pAllocator) {
        // Get destroy function pointer
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
            instance, "vkDestroyDebugUtilsMessengerEXT");

        // Run it if defined
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    VkApplicationInfo VulkanRenderer::makeApplicationInfo() {
        // Set App info
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.apiVersion = VK_API_VERSION_1_3;
        appInfo.pApplicationName = "Vulkan App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Presto";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

        return appInfo;
    }

    void VulkanRenderer::populateDebugMessengerCreateInfo(
        VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
        createInfo = {};
        createInfo.sType =
            VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity =
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType =
            VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;

        // Configurable void* user data
        createInfo.pUserData = nullptr;
    }

    std::vector<const char*> VulkanRenderer::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions =
            glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        // Copies required extensions from glfwExtensions (start
        // pointer) up until glfwExtensions + glfwExtensionCount
        // (Pointer at end)
        std::vector<const char*> extensions(
            glfwExtensions, glfwExtensions + glfwExtensionCount);

        // Add extra extensions
        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData) {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            PR_CORE_ERROR("{}: {}", "VulkanRenderer", pCallbackData->pMessage);
        } else if (messageSeverity >=
                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
            PR_CORE_WARN("{}: {}", "VulkanRenderer", pCallbackData->pMessage);
        } else if (messageSeverity >=
                   VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
            PR_CORE_INFO("{}: {}", "VulkanRenderer", pCallbackData->pMessage);
        } else {
            PR_CORE_TRACE("{}: {}", "VulkanRenderer", pCallbackData->pMessage);
        }

        // False as we dont want to cancel
        return VK_FALSE;
    }

}  // namespace Presto