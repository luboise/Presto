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

       private:
        GLFWwindow* _glfwWindow;

        VkInstance _instance;
        VkDebugUtilsMessengerEXT _debugMessenger;
        VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
        VkDevice _logicalDevice;

        VkSwapchainKHR _swapchain = VK_NULL_HANDLE;
        std::vector<VkImage> _swapchainImages;
        VkFormat _swapchainImageFormat;
        VkExtent2D _swapchainExtent;

        VkQueue _graphicsQueue;
        VkQueue _presentQueue;
        VkSurfaceKHR _surface;

        bool isDeviceSuitable(const VkPhysicalDevice& device);
        QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device);

        // High level init functions
        PR_RESULT createInstance();
        PR_RESULT setupDebugMessenger();
        PR_RESULT createSurface();
        PR_RESULT pickPhysicalDevice();
        PR_RESULT createLogicalDevice();
        PR_RESULT createSwapChain();

        // Low level init functions
        VkApplicationInfo makeApplicationInfo();

        void populateDebugMessengerCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();
        bool checkDeviceExtensionSupport(const VkPhysicalDevice& device) const;
        void initialiseVulkanExtensions();

        SwapChainSupportDetails querySwapChainSupport(
            VkPhysicalDevice device) const;

        VkSurfaceFormatKHR chooseSwapSurfaceFormat(
            const std::vector<VkSurfaceFormatKHR>& availableFormats);

        VkPresentModeKHR chooseSwapPresentMode(
            const std::vector<VkPresentModeKHR>& availablePresentModes);

        VkExtent2D chooseSwapExtent(
            const VkSurfaceCapabilitiesKHR& capabilities);

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