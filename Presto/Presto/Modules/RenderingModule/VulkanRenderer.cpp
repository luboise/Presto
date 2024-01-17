#include "VulkanRenderer.h"

#include <GLFW/glfw3.h>

namespace Presto {
    VulkanRenderer::VulkanRenderer() {
        PR_RESULT result = this->Init();

        if (result == PR_SUCCESS) {
            this->_initialised = true;
        } else {
            this->_initialised = false;
        }
    }

    VulkanRenderer::~VulkanRenderer() { this->Shutdown(); }

    PR_RESULT VulkanRenderer::Init() {
        // Set App info
        VkApplicationInfo appInfo{};

        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.apiVersion = VK_API_VERSION_1_3;

        appInfo.pApplicationName = "Vulkan App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Presto";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

        // Set instance info
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions =
            glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        // TODO: Implement global validation layers
        createInfo.enabledLayerCount = 0;

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
        PR_INFO(ss.str());

        // Create and check instance
        VkResult result =
            vkCreateInstance(&createInfo, nullptr, &(this->instance));
        PR_ASSERT(result == VK_SUCCESS, "Unable to create Vulkan instance.");
        PR_INFO("Created Vulkan instance.");

        PR_RESULT pr_result = PR_FAILURE;

        if (result == VK_SUCCESS) {
            pr_result = PR_SUCCESS;
        }

        return pr_result;
    }

    void VulkanRenderer::Shutdown() { vkDestroyInstance(instance, nullptr); }

}  // namespace Presto