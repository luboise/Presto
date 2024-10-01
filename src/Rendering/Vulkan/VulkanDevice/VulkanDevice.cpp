#include <vulkan/vulkan_core.h>

#include "Rendering/Vulkan/VulkanDevice/VulkanSyncSet/VulkanSyncSet.h"

#include "Rendering/Vulkan/Abstractions/CommandPool.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanConstants.h"
#include "Rendering/Vulkan/VulkanUtils/VulkanUtils.h"
#include "VulkanDevice.h"

#include <Rendering/Vulkan/Abstractions/Buffer.h>

#include <set>

namespace Presto {

    VulkanDevice::VulkanDevice(const VkPhysicalDevice& phys,
                               const VkSurfaceKHR& surface)
        : Abstraction(),
          _indices(QueueFamilyIndices::findQueueFamilies(phys, surface)),
          _physicalDevice(phys),
          _surface(surface) {
        this->createLogicalDevice();
        this->retrieveQueueHandles();
    }

    VulkanDevice::~VulkanDevice() {
        vkDeviceWaitIdle(_handle);
        vkDestroyDevice(_handle, nullptr);
    }

    void VulkanDevice::createLogicalDevice() {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {
            _indices.graphicsFamily.value(), _indices.presentFamily.value()};

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
        queueCreateInfo.queueFamilyIndex = _indices.graphicsFamily.value();
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

        if (vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_handle) !=
            VK_SUCCESS) {
            throw std::runtime_error("Unable to create logical device.");
        }
    }

    void VulkanDevice::retrieveQueueHandles() {
        vkGetDeviceQueue(_handle, _indices.graphicsFamily.value(), 0,
                         &this->_graphicsQueue);
        vkGetDeviceQueue(_handle, _indices.presentFamily.value(), 0,
                         &this->_presentQueue);
    }

    SwapchainSupportDetails VulkanDevice::getSwapchainSupportDetails() const {
        return VulkanDevice::querySwapchainSupport(_physicalDevice, _surface);
    }

    const VkPhysicalDevice& VulkanDevice::getPhysicalDevice() const {
        return this->_physicalDevice;
    };

    const QueueFamilyIndices& VulkanDevice::getQueueFamilyIndices() const {
        return this->_indices;
    }

    // Static Methods

    SwapchainSupportDetails VulkanDevice::querySwapchainSupport(
        VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
        SwapchainSupportDetails details;

        // Read surface capabilities into VkSurfaceCapabilitiesKHR
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface,
                                                  &details.capabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface,
                                             &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                physicalDevice, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface,
                                                  &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                physicalDevice, surface, &presentModeCount,
                details.presentModes.data());
        }

        return details;
    };

    Buffer* VulkanDevice::createBuffer(const Buffer::BUFFER_TYPE type,
                                       const VkDeviceSize size) {
        auto* new_buffer = new Buffer(*this, type, size);
        _buffers.push_back(new_buffer);

        // Fix this to be using the one from the vector
        return new_buffer;
    }

    DescriptorPool* VulkanDevice::createDescriptorPool() {
        // TODO: Fix magic constant
        auto* pool = new DescriptorPool(*this);

        this->_descriptorPools.push_back(pool);
        return pool;
    };

    CommandPool* VulkanDevice::createCommandPool() {
        auto* pool = new CommandPool(*this);

        _commandPools.push_back(pool);

        return pool;
    };

    VulkanSyncSet* VulkanDevice::createSyncSet() {
        auto* sync_set = new VulkanSyncSet(this->_handle);

        this->_syncSets.push_back(sync_set);
        return sync_set;
    };

    void VulkanDevice::submitToGraphicsQueue(
        const std::vector<VkCommandBuffer>& buffers) const {
        VkSubmitInfo submit_info{};
        submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submit_info.commandBufferCount = buffers.size();
        submit_info.pCommandBuffers = buffers.data();

        // Wait for submission (can use a fence instead for multiple
        // transfers)
        vkQueueSubmit(_graphicsQueue, 1, &submit_info, VK_NULL_HANDLE);
        vkQueueWaitIdle(_graphicsQueue);
    }

    VkDeviceMemory VulkanDevice::allocate(Image* image) {
        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(_handle, image->handle(),
                                     &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = VulkanUtils::findMemoryType(
            _physicalDevice, memRequirements.memoryTypeBits,
            image.getMemoryFlags());

        VkDeviceMemory memory = VK_NULL_HANDLE;

        if (vkAllocateMemory(_handle, &allocInfo, nullptr, &memory) !=
            VK_SUCCESS) {
            throw std::runtime_error(
                "Unable to allocate image memory, possibly due to an "
                "overflow.");
        }

        return memory;
    }
}
}
;  // namespace Presto
