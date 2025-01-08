#include "../Swapchain.h"
#include "Presto/Rendering/Renderer.h"

#include "GLFWAppWindow.h"

namespace Presto {
    // Public
    Swapchain::Swapchain(VulkanDevice* device, const VkSurfaceKHR* surface,

                         const GLFWAppWindow* window)
        : Abstraction(), _window(window), _device(device), _surface(surface) {
        this->initialise();
    }

    void Swapchain::initialise() {
        this->createSwapchain();
        this->createImageViews();
    };

    void Swapchain::reload() {
        int width = 0;
        int height = 0;

        // TODO: Move this logic out
        auto* handle = _window->getWindowHandle();

        glfwGetFramebufferSize(handle, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(handle, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(_device->handle());

        this->teardown();
        this->initialise();
    }

    // Private
    VkSurfaceFormatKHR Swapchain::chooseSwapSurfaceFormat(
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

    VkPresentModeKHR Swapchain::chooseSwapPresentMode(
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

    VkExtent2D Swapchain::chooseSwapExtent(
        const VkSurfaceCapabilitiesKHR& capabilities) {
        // Equal to (unsigned)-1 when we should select best ourself
        if (capabilities.currentExtent.width !=
            std::numeric_limits<uint32_t>::max()) {
            return capabilities.currentExtent;
        }

        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(_window->getWindowHandle(), &width, &height);

        // Get size of framebuffer (size of window)
        VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                                   static_cast<uint32_t>(height)};

        // Clamp this to the valid range supported by the device
        actualExtent.width =
            std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                       capabilities.maxImageExtent.width);
        actualExtent.height =
            std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                       capabilities.maxImageExtent.height);

        return actualExtent;
    }

    void Swapchain::teardown() {
        auto* device = _device->handle();

        for (auto& _imageView : _imageViews) {
            vkDestroyImageView(device, _imageView, nullptr);
        }

        vkDestroySwapchainKHR(device, this->_handle, nullptr);
    }

    void Swapchain::createSwapchain() {
        SwapchainSupportDetails swapChainSupport =
            _device->getSwapchainSupportDetails();

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
        createInfo.surface = *_surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;

        createInfo.imageExtent = swapExtent;

        // Higher for stereoscopic 3D
        createInfo.imageArrayLayers = 1;

        // Render it straight (colour attachment). Could change this later to do
        // post processing
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        auto indices = _device->getQueueFamilyIndices();

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

        if (vkCreateSwapchainKHR(_device->handle(), &createInfo, nullptr,
                                 &(this->_handle)) != VK_SUCCESS) {
            throw std::runtime_error("Unable to create Vulkan swapchain.");
        }

        vkGetSwapchainImagesKHR(_device->handle(), this->_handle, &imageCount,
                                nullptr);
        _images.resize(imageCount);

        vkGetSwapchainImagesKHR(_device->handle(), this->_handle, &imageCount,
                                _images.data());

        this->_imageFormat = surfaceFormat.format;
        this->_extent = swapExtent;
    };

    void Swapchain::createImageViews() {
        _imageViews.resize(_images.size());

        for (size_t i = 0; i < _images.size(); i++) {
            VkImageViewCreateInfo createInfo{};

            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = _images[i];

            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = _imageFormat;

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

            if (vkCreateImageView(_device->handle(), &createInfo, nullptr,
                                  &(_imageViews[i])) != VK_SUCCESS) {
                throw std::runtime_error("Unable to create image views.");
            }
        };
    }

}  // namespace Presto
