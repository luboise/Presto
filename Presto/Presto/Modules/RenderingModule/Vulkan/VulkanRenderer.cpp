#include "VulkanRenderer.h"
#include "Presto/Components/_ComponentHeader.h"

#include <algorithm>
#include <limits>
#include <set>

namespace Presto {

    VulkanRenderer::VulkanRenderer(GLFWwindow* window) : _glfwWindow(window) {
        try {
            this->createInstance();
            this->setupDebugMessenger();
            this->createSurface();
            this->pickPhysicalDevice();
            this->createLogicalDevice();
            this->createSwapChain();
            this->createImageViews();

            this->createRenderPass();

            this->createFrameBuffers();
            this->createCommandPool();

            // Buffers
            this->createBuffers();
            // this->initialiseBuffers();
            createDescriptorSetLayout();
            this->createDescriptorPool();
            this->createDescriptorSets();

            this->createCommandBuffers();
            this->createSyncObjects();

            this->createDefaultPipeline();
        } catch (const std::runtime_error& e) {
            PR_CORE_ERROR(
                "Unable to initialise Vulkan renderer. Runtime error: {}",
                e.what());
        }
    }

    VulkanRenderer::~VulkanRenderer() { this->Shutdown(); }

    void VulkanRenderer::Shutdown() {
        vkDeviceWaitIdle(_logicalDevice);
        vkDestroyCommandPool(_logicalDevice, _commandPool, nullptr);

        // Cleanup swapchain has vkDeviceWaitIdle call inside
        cleanupSwapChain();

        // Cleanup uniform buffers
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyBuffer(_logicalDevice, _uniformBuffers[i], nullptr);
            vkFreeMemory(_logicalDevice, _uniformBuffersMemory[i], nullptr);
        }

        vkDestroyDescriptorPool(_logicalDevice, _descriptorPool, nullptr);

        vkDestroyDescriptorSetLayout(_logicalDevice, _descriptorSetLayout,
                                     nullptr);

        vkDestroyBuffer(_logicalDevice, _vertexBuffer, nullptr);
        vkFreeMemory(_logicalDevice, _vertexBufferMemory, nullptr);

        vkDestroyBuffer(_logicalDevice, _indexBuffer, nullptr);
        vkFreeMemory(_logicalDevice, _indexBufferMemory, nullptr);

        vkDestroyBuffer(_logicalDevice, _stagingBuffer, nullptr);
        vkFreeMemory(_logicalDevice, _stagingBufferMemory, nullptr);

        for (auto& pipeline : _graphicsPipelines) {
            vkDestroyPipeline(_logicalDevice, pipeline.graphicsPipeline,
                              nullptr);
            vkDestroyPipelineLayout(_logicalDevice, pipeline.pipelineLayout,
                                    nullptr);
        }

        vkDestroyRenderPass(_logicalDevice, _renderPass, nullptr);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(_logicalDevice, _renderFinishedSemaphores[i],
                               nullptr);
            vkDestroySemaphore(_logicalDevice, _imageAvailableSemaphores[i],
                               nullptr);
            vkDestroyFence(_logicalDevice, _inFlightFences[i], nullptr);
        }

        vkDestroyDevice(_logicalDevice, nullptr);

        this->_logicalDevice = nullptr;

        if (enableValidationLayers) {
            this->DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger,
                                                nullptr);
            this->_debugMessenger = nullptr;
        }

        vkDestroySurfaceKHR(_instance, _surface, nullptr);
        vkDestroyInstance(_instance, nullptr);
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

    void VulkanRenderer::createInstance() {
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error(
                "Validation layers not supported, but checks for validation "
                "layers are on.");
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
        if (vkCreateInstance(&createInfo, nullptr, &(this->_instance)) !=
            VK_SUCCESS) {
            throw std::runtime_error("Unable to create Vulkan instance.");
        }
        PR_CORE_INFO("Created Vulkan instance.");
    }

    void VulkanRenderer::setupDebugMessenger() {
        // Return if no validation layers
        if (!enableValidationLayers) return;

        VkDebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        if (CreateDebugUtilsMessengerEXT(this->_instance, &createInfo, nullptr,
                                         &this->_debugMessenger) !=
            VK_SUCCESS) {
            throw std::runtime_error("Unable to create debug utils messenger.");
        }
    }

    void VulkanRenderer::createDefaultPipeline() {
        VulkanPipeline pipeline;

        buildGraphicsPipeline(pipeline);
        // createGraphicsPipeline(options);

        _graphicsPipelines.push_back(pipeline);
    }

    void VulkanRenderer::createBuffer(VkDeviceSize size,
                                      VkBufferUsageFlags usageFlags,
                                      VkMemoryPropertyFlags propFlags,
                                      VkBuffer& buffer,
                                      VkDeviceMemory& bufferMemory) {
        // Create buffer
        VkBufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.size = size;
        createInfo.usage = usageFlags;
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(_logicalDevice, &createInfo, nullptr, &buffer)) {
            throw std::runtime_error("Failed to create vertex buffer.");
        }

        // Allocate memory on GPU

        // Get memory requirements of device to allocate memory
        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(_logicalDevice, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;

        allocInfo.memoryTypeIndex =
            findMemoryType(memRequirements.memoryTypeBits, propFlags);

        if (vkAllocateMemory(_logicalDevice, &allocInfo, nullptr,
                             &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error(
                "Failed to allocate vertex buffer memory.");
        }

        // Bind allocate memory to buffer (at offset 0, otherwise must be
        // divisible by memRequirements.alignment)
        vkBindBufferMemory(_logicalDevice, buffer, bufferMemory, 0);
    }

    uint32_t VulkanRenderer::findMemoryType(uint32_t typeFilter,
                                            VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            // Checks i'th bit from right
            if ((typeFilter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties)) {
                return i;
            }
        }

        // Error value
        return -1;
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

    void VulkanRenderer::createDescriptorSetLayout() {
        // Create binding
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = 0;
        layoutBinding.descriptorCount = 1;
        layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        // Create descriptor set with binding
        VkDescriptorSetLayoutCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        createInfo.pNext = NULL;
        createInfo.bindingCount = 1;
        createInfo.pBindings = &layoutBinding;

        if (vkCreateDescriptorSetLayout(_logicalDevice, &createInfo, nullptr,
                                        &_descriptorSetLayout) != VK_SUCCESS) {
            throw std::runtime_error("Unable to create descriptor set layout.");
        };
    }

    void VulkanRenderer::pickPhysicalDevice() {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(this->_instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error(
                "Unable to find a Vulkan compatible graphics "
                "device.");
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
            throw std::runtime_error("No suitable graphics device found.");
        }
    }

    void VulkanRenderer::createLogicalDevice() {
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
            throw std::runtime_error("Unable to create logical device.");
        }

        vkGetDeviceQueue(_logicalDevice, indices.graphicsFamily.value(), 0,
                         &_graphicsQueue);
        vkGetDeviceQueue(_logicalDevice, indices.presentFamily.value(), 0,
                         &_presentQueue);
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

    void VulkanRenderer::AddToRenderPool(entity_t entity_ptr) {
        if (_entityMap.contains(entity_ptr)) {
            PR_CORE_ERROR(
                "Entity has already been added to render pool. Can't add it "
                "again.");
            return;
        }

        VulkanPipeline& pipeline = _graphicsPipelines[0];

        for (auto& pair : entity_ptr->getComponents()) {
            Polygon2D* polygon = dynamic_cast<Polygon2D*>(pair.second);
            if (polygon != nullptr) {
                const VertexList& vertices = polygon->getVertices();
                const IndexList& indices = polygon->getIndices();

                VkDeviceSize bufferSize;
                void* data;

                // Copy vertices into staging buffer
                bufferSize = sizeof(VulkanVertex) * ALLOCATED_VERTICES;
                vkMapMemory(_logicalDevice, _stagingBufferMemory, 0, bufferSize,
                            0, &data);
                memcpy(data, vertices.data(),
                       sizeof(VulkanVertex) * vertices.size());
                vkUnmapMemory(_logicalDevice, _stagingBufferMemory);

                copyBuffer(_stagingBuffer, _vertexBuffer, bufferSize);

                // Copy indices into index buffer
                bufferSize = sizeof(uint32_t) * ALLOCATED_INDICES;
                vkMapMemory(_logicalDevice, _stagingBufferMemory, 0, bufferSize,
                            0, &data);
                memcpy(data, indices.data(), sizeof(uint32_t) * indices.size());
                vkUnmapMemory(_logicalDevice, _stagingBufferMemory);

                copyBuffer(_stagingBuffer, _indexBuffer, bufferSize);

                auto info = DrawInfo{};

                info.indexCount = indices.size();
                info.ibOffset = 0;

                info.vertexCount = vertices.size();
                info.vbOffset = 0;

                _entityMap.try_emplace(entity_ptr, info);
                break;
            }
        }
    }
}  // namespace Presto