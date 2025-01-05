#pragma once

#include <vulkan/vulkan_core.h>
#include "PrestoCore/Rendering/Renderer.h"
#include "Rendering/Vulkan/VulkanDevice/VulkanDevice.h"

#include "Abstraction.h"

namespace PrestoCore {
    class VulkanDevice;
    /*
       A wrapper for a Vulkan swapchain.
Params:
VulkanDevice - The logical device used to draw to the swapchain.
VkSurfaceKHR - The surface that the swapchain belongs to.
       */
    class Swapchain : public Abstraction<VkSwapchainKHR> {
       public:
        Swapchain(VulkanDevice*, const VkSurfaceKHR*, const GLFWAppWindow*);
        ~Swapchain() { this->teardown(); };

        explicit operator VkSwapchainKHR() const { return this->_handle; }

        [[nodiscard]] inline VkExtent2D getExtent() const {
            return this->_extent;
        };
        [[nodiscard]] inline VulkanDevice* getDevice() const {
            return this->_device;
        };
        [[nodiscard]] inline const VkFormat& getImageFormat() const {
            return this->_imageFormat;
        };
        [[nodiscard]] const std::vector<VkImageView>& getImageViews() const {
            return this->_imageViews;
        };

        VkResult getNextImage(uint32_t& _image_out,
                              VkSemaphore wait_for = nullptr) {
            // Acquire image from swap chain to draw into
            VkResult res =
                vkAcquireNextImageKHR(_device->handle(), _handle, UINT64_MAX,
                                      wait_for, VK_NULL_HANDLE, &_image_out);

            return res;
        }

        void reload();

       private:
        SwapchainSupportDetails querySwapChainSupport(VulkanDevice&);

        void initialise();

        void createSwapchain();
        void createImageViews();

        void teardown();

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(
            const VkSurfaceCapabilitiesKHR& capabilities);
        const GLFWAppWindow* _window;

        VulkanDevice* _device;
        const VkSurfaceKHR* _surface;

        VkFormat _imageFormat;
        VkExtent2D _extent;

        std::vector<VkImage> _images;
        std::vector<VkImageView> _imageViews;

        VkImage _depthImage;
        VkImageView _depthImageView;
    };
}  // namespace PrestoCore
