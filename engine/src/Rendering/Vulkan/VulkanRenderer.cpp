#include <vulkan/vulkan_core.h>
#include <algorithm>

#include "Presto/Core/Constants.h"
#include "Rendering/Renderer.h"

#include "Presto/Runtime/GLFWAppWindow.h"

#include "Abstractions/Buffer.h"
#include "Abstractions/Pipeline.h"
#include "Abstractions/Swapchain.h"

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

VulkanRenderer::VulkanRenderer(GLFWAppWindow* window) {
    this->setWindow(window);

    try {
        this->initialiseSystems();

        // Create binding
        VkDescriptorSetLayoutBinding globalBindingDetails{};
        globalBindingDetails.binding = 0;
        globalBindingDetails.descriptorCount = 1;
        globalBindingDetails.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        globalBindingDetails.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        // Create binding
        VkDescriptorSetLayoutBinding localBindingDetails{};
        localBindingDetails.binding = 0;
        localBindingDetails.descriptorCount = 1;
        localBindingDetails.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        localBindingDetails.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        auto* global_layout = descriptorManager_->createDescriptorSetLayout(
            {globalBindingDetails});
        auto* local_layout = descriptorManager_->createDescriptorSetLayout(
            {localBindingDetails});

        auto* global_group = descriptorManager_->createDescriptorGroup(
            {global_layout, local_layout}, MAX_FRAMES_IN_FLIGHT,
            *bufferManager_);

        RenderContext* layer = drawManager_->addLayer(MAIN_LAYER_NAME);

        PipelineBuilder pipeline_builder =
            pipelineManager_->getBuilder(*global_group);

        for (const auto& desc : VulkanVertex::getAttributeDescriptions()) {
            pipeline_builder.addVertexAttributeDescription(desc);
        }

        pipeline_builder
            .addVertexBindingDescription(VulkanVertex::getBindingDescription())
            .setRenderContext(*layer)
            .addShader(new VulkanShader(_device->handle(),
                                        SHADER_MODULE_TYPE::FRAGMENT,
                                        "Shaders/Core/basic_triangle_frag.spv"))
            .addShader(
                new VulkanShader(_device->handle(), SHADER_MODULE_TYPE::VERTEX,
                                 "Shaders/Core/basic_triangle_vert.spv"));

        layer->setPipeline(pipeline_builder.createPipeline());
    } catch (const std::runtime_error& e) {
        PR_CORE_ERROR("Unable to initialise Vulkan renderer. Runtime error: {}",
                      e.what());
    }
}

Allocated<Buffer> VulkanRenderer::createBuffer(Buffer::BufferType type,
                                               Presto::size_t size) {

};

void VulkanRenderer::initialiseSystems() {
    instance_ = VulkanUtils::getNewVulkanInstance();
    surface_ = this->createSurface();

    VulkanUtils::setupDebugMessenger(instance_, &debugMessenger_);

    _physicalDevices = this->getPhysicalDevices(_instance, _surface);
    if (_physicalDevices.empty() || _physicalDevices[0] == VK_NULL_HANDLE) {
        throw std::runtime_error("No compatible Vulkan device was found.");
    }
    _physicalDevice = _physicalDevices[0];

    this->device_ = new VulkanDevice(physicalDevice_, surface_);
    this->swapchain_ = new Swapchain(device_, &surface_, _glfwWindow);

    // Initialises all things to do with vertex/index buffers
    this->bufferManager_ = new BufferManager(device_);

    // Handles descriptors
    this->descriptorManager_ = new DescriptorManager(*device_, *bufferManager_);

    // Create draw manager with MAX_FRAMES_IN_FLIGHT contexts (render
    // passes)
    this->drawManager_ = new DrawManager(*swapchain_, MAX_FRAMES_IN_FLIGHT);
    this->pipelineManager_ = new PipelineManager(device_);
}

void VulkanRenderer::addToRenderPool(draw_info_key renderable) {
    // TODO: Make this layer agnostic
    auto* pipeline = drawManager_->getLayer(MAIN_LAYER_NAME)->getPipeline();

    bufferManager_->loadRenderable(renderable, pipeline);
}

void VulkanRenderer::render(mesh_context_id_t* context) {
    // Wait for previous frame (1 fence, wait all fences) then reset
    // fence to unsignaled

    const VulkanDrawInfo& info = bufferManager_->getDrawInfo(r);
    drawManager_->draw(info);
}

void VulkanRenderer::nextFrame() {
    // Update the camera, then send the draw commands

    // TODO: Move this into a module in the renderer
    // Update uniform buffers

    auto current_frame = drawManager_->getCurrentFrame();

    const auto& buffers = drawManager_->getLayer(MAIN_LAYER_NAME)
                              ->getPipeline()
                              ->getDescriptors()
                              .getFrameSet(current_frame)
                              .uniform_buffers;

    drawManager_->goToNextFrame();
}

VulkanRenderer::~VulkanRenderer() { this->VulkanRenderer::Shutdown(); }

void VulkanRenderer::Shutdown() {
    delete drawManager_;
    delete pipelineManager_;
    delete bufferManager_;
    delete descriptorManager_;

    delete swapchain_;
    delete device_;

    if (enableValidationLayers) {
        VulkanUtils::DestroyDebugUtilsMessengerEXT(instance_, debugMessenger_,
                                                   nullptr);
        this->debugMessenger_ = nullptr;
    }

    vkDestroySurfaceKHR(instance_, surface_, nullptr);
    vkDestroyInstance(instance_, nullptr);
}

bool VulkanRenderer::isDeviceSuitable(const VkPhysicalDevice& device,
                                      const VkSurfaceKHR& surface) {
    auto indices = QueueFamilyIndices::findQueueFamilies(device, surface);

    bool extensionsSupported = VulkanUtils::checkDeviceExtensionSupport(device);

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
                             return VulkanRenderer::isDeviceSuitable(device,
                                                                     surface);
                         });

    return compatibleDevices;
}

VkSurfaceKHR VulkanRenderer::createSurface() const {
    VkSurfaceKHR surface = nullptr;

    if (glfwCreateWindowSurface(
            instance_, static_cast<GLFWwindow*>(_glfwWindow->getWindowHandle()),
            nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("Unable to create window surface.");
    }
    return surface;
}

VulkanDevice& VulkanRenderer::getDevice() { return *VulkanRenderer::device; };

bool VulkanRenderer::createMeshContext(
    MeshRegistrationData& registration,
    const PipelineStructure& structure) override {};

}  // namespace Presto
