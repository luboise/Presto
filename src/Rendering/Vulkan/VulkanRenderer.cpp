#include <vulkan/vulkan_core.h>
#include <algorithm>

#include "Presto/Rendering/Renderer.h"

#include "Abstractions/Buffer.h"
#include "Abstractions/Pipeline.h"
#include "Abstractions/Swapchain.h"
#include "GLFWAppWindow.h"
#include "Managers/DrawManager/DrawManager.h"
#include "Managers/PipelineManager/PipelineManager.h"
#include "Rendering/Utils/RenderingUtils.h"
#include "Rendering/Vulkan/Managers/PipelineManager/PipelineBuilder/PipelineBuilder.h"
#include "VulkanDevice/VulkanDevice.h"
#include "VulkanRenderer.h"

#include "Managers/BufferManager/BufferManager.h"
#include "Managers/DescriptorManager/DescriptorManager.h"
#include "VulkanUtils/VulkanConstants.h"
#include "VulkanUtils/VulkanTypes.h"
#include "VulkanUtils/VulkanUtils.h"
#include "VulkanUtils/VulkanVertex.h"

namespace Presto {
    using namespace VulkanUtils;

    void VulkanRenderer::addToRenderPool(draw_info_key renderable) {
        // TODO: Make this layer agnostic
        auto* pipeline = _drawManager->getLayer(MAIN_LAYER_NAME)->getPipeline();

        _bufferManager->loadRenderable(renderable, pipeline);
    }

    void VulkanRenderer::render(Renderable* const r) {
        // Wait for previous frame (1 fence, wait all fences) then reset
        // fence to unsignaled

        const VulkanDrawInfo& info = _bufferManager->getDrawInfo(r);
        _drawManager->draw(info);
    }

    void VulkanRenderer::nextFrame() {
        // Update the camera, then send the draw commands

        // TODO: Move this into a module in the renderer
        // Update uniform buffers

        auto current_frame = _drawManager->getCurrentFrame();

        const auto& buffers = _drawManager->getLayer(MAIN_LAYER_NAME)
                                  ->getPipeline()
                                  ->getDescriptors()
                                  .getFrameSet(current_frame)
                                  .uniform_buffers;

        ShaderMatrices mats{};

        glm::mat4 model(1.0);
        constexpr glm::float32 FOV_Y = 90;
        auto extent = _swapchain->getExtent();

        mats.modelView = _renderCamera->getViewMatrix() * model;
        mats.projection = RenderingUtils::getProjectionMatrix(
            FOV_Y, extent.width, extent.height);

        for (const auto& buffer : buffers) {
            buffer->write(sizeof(mats), &mats);
        }

        _drawManager->goToNextFrame();
    }

    VulkanRenderer::VulkanRenderer(GLFWAppWindow* window) {
        this->setWindow(window);

        try {
            this->initialiseSystems();

            // Create binding
            VkDescriptorSetLayoutBinding globalBindingDetails{};
            globalBindingDetails.binding = 0;
            globalBindingDetails.descriptorCount = 1;
            globalBindingDetails.descriptorType =
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            globalBindingDetails.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            // Create binding
            VkDescriptorSetLayoutBinding localBindingDetails{};
            localBindingDetails.binding = 0;
            localBindingDetails.descriptorCount = 1;
            localBindingDetails.descriptorType =
                VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            localBindingDetails.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

            auto* global_layout = _descriptorManager->createDescriptorSetLayout(
                {globalBindingDetails});
            auto* local_layout = _descriptorManager->createDescriptorSetLayout(
                {localBindingDetails});

            auto* global_group = _descriptorManager->createDescriptorGroup(
                {global_layout, local_layout}, MAX_FRAMES_IN_FLIGHT,
                *_bufferManager);

            RenderContext* layer = _drawManager->addLayer(MAIN_LAYER_NAME);

            PipelineBuilder pipeline_builder =
                _pipelineManager->getBuilder(*global_group);

            for (const auto& desc : VulkanVertex::getAttributeDescriptions()) {
                pipeline_builder.addVertexAttributeDescription(desc);
            }

            pipeline_builder
                .addVertexBindingDescription(
                    VulkanVertex::getBindingDescription())
                .setRenderContext(*layer)
                .addShader(new VulkanShader(
                    _device->handle(), SHADER_MODULE_TYPE::FRAGMENT,
                    "Shaders/Core/basic_triangle_frag.spv"))
                .addShader(new VulkanShader(
                    _device->handle(), SHADER_MODULE_TYPE::VERTEX,
                    "Shaders/Core/basic_triangle_vert.spv"));

            layer->setPipeline(pipeline_builder.createPipeline());
        } catch (const std::runtime_error& e) {
            PR_CORE_ERROR(
                "Unable to initialise Vulkan renderer. Runtime error: {}",
                e.what());
        }
    }

    void VulkanRenderer::initialiseSystems() {
        _instance = getNewVulkanInstance();
        _surface = createSurface();

        setupDebugMessenger(_instance, &_debugMessenger);

        _physicalDevices = getPhysicalDevices(_instance, _surface);
        if (_physicalDevices.empty() || _physicalDevices[0] == VK_NULL_HANDLE) {
            throw std::runtime_error("No compatible Vulkan device was found.");
        }
        _physicalDevice = _physicalDevices[0];

        this->_device = new VulkanDevice(_physicalDevice, _surface);
        this->_swapchain = new Swapchain(_device, &_surface, _glfwWindow);

        // Initialises all things to do with vertex/index buffers
        this->_bufferManager = new BufferManager(_device);

        // Handles descriptors
        this->_descriptorManager =
            new DescriptorManager(*_device, *_bufferManager);

        // Create draw manager with MAX_FRAMES_IN_FLIGHT contexts (render
        // passes)
        this->_drawManager = new DrawManager(*_swapchain, MAX_FRAMES_IN_FLIGHT);
        this->_pipelineManager = new PipelineManager(_device);
    }

    VulkanRenderer::~VulkanRenderer() { this->VulkanRenderer::Shutdown(); }

    void VulkanRenderer::Shutdown() {
        delete _drawManager;
        delete _pipelineManager;
        delete _bufferManager;
        delete _descriptorManager;

        delete _swapchain;
        delete _device;

        if (enableValidationLayers) {
            DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
            this->_debugMessenger = nullptr;
        }

        vkDestroySurfaceKHR(_instance, _surface, nullptr);
        vkDestroyInstance(_instance, nullptr);
    }

    bool VulkanRenderer::isDeviceSuitable(const VkPhysicalDevice& device,
                                          const VkSurfaceKHR& surface) {
        auto indices = QueueFamilyIndices::findQueueFamilies(device, surface);

        bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapchainAdequate = false;
        if (extensionsSupported) {
            SwapchainSupportDetails swapChainSupport =
                VulkanDevice::querySwapchainSupport(device, surface);

            swapchainAdequate = !swapChainSupport.formats.empty() &&
                                !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapchainAdequate;
    }

    std::vector<VkPhysicalDevice> VulkanRenderer::getPhysicalDevices(
        const VkInstance& instance, const VkSurfaceKHR& surface) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

        if (deviceCount == 0) {
            throw std::runtime_error(
                "Unable to find a Vulkan compatible graphics "
                "device.");
        }

        std::vector<VkPhysicalDevice> allDevices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, allDevices.data());

        std::vector<VkPhysicalDevice> compatibleDevices;

        std::ranges::copy_if(allDevices, std::back_inserter(compatibleDevices),
                             [surface](VkPhysicalDevice device) {
                                 return VulkanRenderer::isDeviceSuitable(
                                     device, surface);
                             });

        return compatibleDevices;
    }

    VkSurfaceKHR VulkanRenderer::createSurface() const {
        VkSurfaceKHR surface = nullptr;

        if (glfwCreateWindowSurface(_instance, _glfwWindow->getWindowHandle(),
                                    nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("Unable to create window surface.");
        }
        return surface;
    }

}  // namespace Presto
