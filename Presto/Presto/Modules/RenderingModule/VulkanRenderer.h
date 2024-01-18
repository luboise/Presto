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

        // Low level init functions
        VkApplicationInfo makeApplicationInfo();

        void populateDebugMessengerCreateInfo(
            VkDebugUtilsMessengerCreateInfoEXT& createInfo);

        std::vector<const char*> getRequiredExtensions();
        bool checkValidationLayerSupport();
        void initialiseVulkanExtensions();

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