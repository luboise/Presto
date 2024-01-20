#include "VulkanRenderer.h"
#include "Presto/Modules/ResourcesModule/ResourceManager.h"

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
        if (res == PR_SUCCESS) {
            res = this->createImageViews();
        }
        if (res == PR_SUCCESS) {
            res = this->createRenderPass();
        }
        if (res == PR_SUCCESS) {
            res = this->createGraphicsPipeline();
        }
        this->_initialised = (res == PR_SUCCESS) ? true : false;
    }

    void VulkanRenderer::Shutdown() {
        vkDestroyPipelineLayout(_logicalDevice, _pipelineLayout, nullptr);
        vkDestroyRenderPass(_logicalDevice, _renderPass, nullptr);

        for (auto imageView : _swapchainImageViews) {
            vkDestroyImageView(_logicalDevice, imageView, nullptr);
        }

        vkDestroySwapchainKHR(_logicalDevice, _swapchain, nullptr);
        _swapchain = nullptr;

        vkDestroySurfaceKHR(_instance, _surface, nullptr);
        _surface = nullptr;

        vkDestroyDevice(_logicalDevice, nullptr);
        this->_logicalDevice = nullptr;

        if (enableValidationLayers) {
            this->DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger,
                                                nullptr);
            this->_debugMessenger = nullptr;
        }

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

    VkShaderModule VulkanRenderer::createShaderModule(
        const std::vector<char>& code) {
        VkShaderModuleCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;

        auto res = vkCreateShaderModule(_logicalDevice, &createInfo, nullptr,
                                        &shaderModule);

        if (res != VK_SUCCESS) {
            PR_CORE_ERROR("Failed to create shader module of size {}",
                          code.size());
        }

        return shaderModule;
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
        }

        vkGetSwapchainImagesKHR(this->_logicalDevice, this->_swapchain,
                                &imageCount, nullptr);
        _swapchainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(this->_logicalDevice, this->_swapchain,
                                &imageCount, _swapchainImages.data());

        this->_swapchainImageFormat = surfaceFormat.format;
        this->_swapchainExtent = swapExtent;

        return PR_SUCCESS;
    }
    PR_RESULT VulkanRenderer::createImageViews() {
        _swapchainImageViews.resize(_swapchainImages.size());

        for (size_t i = 0; i < _swapchainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};

            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = _swapchainImages[i];

            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = _swapchainImageFormat;

            // DONT swap any colour channels
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            // Using image for colour, and no mipmapping
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            VkResult res =
                vkCreateImageView(_logicalDevice, &createInfo, nullptr,
                                  &(_swapchainImageViews[i]));

            if (res != VK_SUCCESS) {
                PR_CORE_CRITICAL("Failed to create image views.");
                return PR_FAILURE;
            }
        };

        return PR_SUCCESS;
    }

    PR_RESULT VulkanRenderer::createRenderPass() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = _swapchainImageFormat;

        // 1 Sample, no multisampling for color attachment
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;

        // Clear to black before drawing
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        // Keep value in memory after computing
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        // Not using stencil buffer
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        // Finalise image for presenting after render pass
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        // This 0 is LOCATION 0
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;

        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;

        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        auto res = vkCreateRenderPass(_logicalDevice, &renderPassInfo, nullptr,
                                      &(this->_renderPass));
    }

    PR_RESULT VulkanRenderer::createGraphicsPipeline() {
        VkGraphicsPipelineCreateInfo createInfo{};

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

        auto vertShaderCode = ResourceManager::readFile("shaders/vert.spv");
        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        vertShaderStageInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";
        // Entrypoint of shader ^

        auto fragShaderCode = ResourceManager::readFile("shaders/frag.spv");
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);
        fragShaderStageInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";
        // Entrypoint of shader ^

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo,
                                                          fragShaderStageInfo};

        createInfo.stageCount = 2;
        createInfo.pStages = shaderStages;

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;

        createInfo.pVertexInputState = &vertexInputInfo;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType =
            VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        createInfo.pInputAssemblyState = &inputAssembly;

        // Dynamic stages (can define later at render time)
        std::vector<VkDynamicState> dynamicStages = {VK_DYNAMIC_STATE_VIEWPORT,
                                                     VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType =
            VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount =
            static_cast<uint32_t>(dynamicStages.size());
        dynamicState.pDynamicStates = dynamicStages.data();

        createInfo.pDynamicState = &dynamicState;

        // Viewport
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType =
            VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;

        createInfo.pViewportState = &viewportState;

        // Rasteriser
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType =
            VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;

        // Wireframe rendering
        rasterizer.depthClampEnable = VK_FALSE;
        // Discard geometry past rasteriser
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        // Fill polygon area with fragments
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        // Line thickness in fragments
        rasterizer.lineWidth = 1.0f;

        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;

        // Depth bias settings
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;  // Optional
        rasterizer.depthBiasClamp = 0.0f;           // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f;     // Optional

        createInfo.pRasterizationState = &rasterizer;

        // Multisampling (currently disabled)
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType =
            VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;           // Optional
        multisampling.pSampleMask = nullptr;             // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE;  // Optional
        multisampling.alphaToOneEnable = VK_FALSE;       // Optional

        createInfo.pMultisampleState = &multisampling;

        // Color blending options
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask =
            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
            VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;

        // POTENTIAL COLOUR BLEND CASE
        // colorBlendAttachment.blendEnable = VK_TRUE;
        // colorBlendAttachment.srcColorBlendFactor =
        // VK_BLEND_FACTOR_SRC_ALPHA;
        // colorBlendAttachment.dstColorBlendFactor =
        //     VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        // colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        // colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        // colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        // colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType =
            VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.attachmentCount = 1;
        createInfo.pColorBlendState = &colorBlending;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType =
            VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        // Number of uniform variables
        pipelineLayoutInfo.setLayoutCount = 0;             // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr;          // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0;     // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr;  // Optional

        auto res = vkCreatePipelineLayout(_logicalDevice, &pipelineLayoutInfo,
                                          nullptr, &_pipelineLayout);

        if (res != VK_SUCCESS) {
            PR_CORE_ERROR("Unable to create graphics pipeline layout.");
            return PR_FAILURE;
        }
        createInfo.layout = this->_pipelineLayout;

        VkRenderPass renderPass;
        uint32_t subpass;
        VkPipeline basePipelineHandle;
        int32_t basePipelineIndex;

        // VkPipelineCreateFlags = flags;
        createInfo.pDepthStencilState = nullptr;
        createInfo.pTessellationState = nullptr;
        VkPipeline pipeline;
        vkCreateGraphicsPipelines(_logicalDevice, nullptr, 2, &shaderStages,
                                  nullptr, &pipeline);

        vkDestroyShaderModule(_logicalDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(_logicalDevice, vertShaderModule, nullptr);

        // STORE
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)_swapchainExtent.width;
        viewport.height = (float)_swapchainExtent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        // Want size thats >= viewport, so it doesn't cut it off at all
        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = _swapchainExtent;
    };

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