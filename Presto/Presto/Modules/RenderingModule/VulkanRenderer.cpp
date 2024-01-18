#include "VulkanRenderer.h"

#include <GLFW/glfw3.h>

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

namespace Presto {
    const std::vector<const char*> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

    VulkanRenderer::VulkanRenderer() { this->Init(); }

    VulkanRenderer::~VulkanRenderer() {
        if (this->IsInitialised()) this->Shutdown();
    };

    void VulkanRenderer::Init() {
        auto res = this->createInstance();
        auto res2 = this->setupDebugMessenger();

        this->_initialised =
            (res == PR_SUCCESS && res2 == PR_SUCCESS) ? true : false;
    }

    void VulkanRenderer::Shutdown() {
        if (enableValidationLayers) {
            this->DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger,
                                                nullptr);
        }
        vkDestroyInstance(_instance, nullptr);

        this->_initialised = false;
        this->_instance = nullptr;
        this->_debugMessenger = nullptr;
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

        // Copies required extensions from glfwExtensions (start pointer) up
        // until glfwExtensions + glfwExtensionCount (Pointer at end)
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