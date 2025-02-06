#pragma once

#include <vulkan/vulkan_core.h>
#include "Presto/Rendering/Renderer.h"

namespace Presto {

class VulkanShader {
    friend class PipelineBuilder;

   public:
    [[nodiscard]] VkPipelineShaderStageCreateInfo getPipelineCreateInfo() const;

    static VkShaderStageFlagBits BitsFromShaderModuleType(
        const SHADER_MODULE_TYPE type);

    operator VkShaderModule() const { return this->_shaderModule; }

    ~VulkanShader();

    VulkanShader(VkDevice, SHADER_MODULE_TYPE, const std::string& filepath);

    VulkanShader(VkDevice, SHADER_MODULE_TYPE, const std::vector<char>& code);

   private:
    // Pointer to allocated Vulkan Shader
    VkDevice _device;
    VkShaderModule _shaderModule = nullptr;
    SHADER_MODULE_TYPE _type;
};
}  // namespace Presto
