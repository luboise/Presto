#include "_GraphicsRenderer.h"

#include <vulkan/vulkan.h>
// KEEP THESE SEPARATE
#include <GLFW/glfw3.h>

#include <optional>

namespace Presto {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return this->graphicsFamily.has_value() &&
                   presentFamily.has_value();
        };
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class PRESTO_API VulkanRenderer : public RenderingModule {
       public:
        VulkanRenderer(GLFWwindow* window);
        virtual ~VulkanRenderer();

        virtual void Init();
        virtual void Shutdown();

        PR_RESULT drawFrame();

       private:
        GLFWwindow* _glfwWindow;

        VkInstance _instance;
        VkDebugUtilsMessengerEXT _debugMessenger;
        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        VkDevice _logicalDevice;

        VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
        VkFormat _swapchainImageFormat;
        VkExtent2D _swapchainExtent;

        std::vector<VkImage> _swapchainImages;
        std::vector<VkImageView> _swapchainImageViews;
        std::vector<VkFramebuffer> _swapchainFramebuffers;

        VkRenderPass _renderPass;
        VkPipelineLayout _pipelineLayout;
        VkPipeline _graphicsPipeline;

        VkQueue _graphicsQueue;
        VkQueue _presentQueue;
        VkSurfaceKHR _surface;

        VkCommandPool _commandPool;
        VkCommandBuffer _commandBuffer;

        VkSemaphore _imageAvailableSemaphore;
        VkSemaphore _renderFinishedSemaphore;
        VkFence _inFlightFence;

        bool isDeviceSuitable(const VkPhysicalDevice& device);
        QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device);

        // High level init functions
        PR_RESULT createInstance();
        PR_RESULT setupDebugMessenger();
        PR_RESULT createSurface();
        PR_RESULT pickPhysicalDevice();
        PR_RESULT createLogicalDevice();
        PR_RESULT createSwapChain();
        PR_RESULT createImageViews();
        PR_RESULT createRenderPass();
        PR_RESULT createGraphicsPipeline();
        PR_RESULT createFrameBuffers();
        PR_RESULT createCommandPool();
        PR_RESULT createCommandBuffer();
        PR_RESULT createSyncObjects();

        PR_RESULT recordCommandBuffer(VkCommandBuffer commandBuffer,
                                      uint32_t imageIndex);

        // Low level init functions
        VkApplicationInfo makeApplicationInfo();

        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();
        bool checkDeviceExtensionSupport(const VkPhysicalDevice& device) const;
        void initialiseVulkanExtensions();

        VkShaderModule createShaderModule(const std::vector<char>& code);

        SwapChainSupportDetails querySwapChainSupport(
            VkPhysicalDevice device) const;

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(
            const VkSurfaceCapabilitiesKHR& capabilities);

        void populateDebugMessengerCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        // Ping Vulkan for creation function
        static VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
            const VkAllocationCallbacks* pAllocator,
            VkDebugUtilsMessengerEXT* pDebugMessenger);

        static void DestroyDebugUtilsMessengerEXT(
            VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks* pAllocator);

        // Debug callback function for Vulkan
        static VKAPI_ATTR VkBool32 VKAPI_CALL
        debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                      VkDebugUtilsMessageTypeFlagsEXT messageType,
                      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                      void* pUserData);
    };
}  // namespace Presto