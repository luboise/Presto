#include "VulkanShader.h"
#include <vulkan/vulkan_core.h>
#include <stdexcept>

#include "PrestoCore/Rendering/Renderer.h"
#include "ResourcesModule/ResourceManager.h"

namespace PrestoCore {
    using ShaderCreateInfo = VkPipelineShaderStageCreateInfo;

    ShaderCreateInfo VulkanShader::getPipelineCreateInfo() const {
        VkPipelineShaderStageCreateInfo stage_info{};

        stage_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

        stage_info.stage = VulkanShader::BitsFromShaderModuleType(_type);

        stage_info.module = this->_shaderModule;
        stage_info.pName = "main";
        // Entrypoint of shader ^

        return stage_info;
    }

    VulkanShader::VulkanShader(VkDevice device, SHADER_MODULE_TYPE type,
                               const std::string& filepath)
        : _device(device) {
        auto code = ResourceManager::ReadFile(filepath);

        PR_ASSERT(code.size() > 0,
                  "Attempted to create a shader from an empty code vector.");

        this->_type = type;

        VkShaderModuleCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(device, &createInfo, nullptr,
                                 &this->_shaderModule) != VK_SUCCESS) {
            std::stringstream ss;

            ss << "Failed to create shader module of size " << code.size()
               << ".";

            throw std::runtime_error(ss.str());
        }
    }

    VkShaderStageFlagBits VulkanShader::BitsFromShaderModuleType(
        const SHADER_MODULE_TYPE type) {
        switch (type) {
            case SHADER_MODULE_TYPE::VERTEX:
                return VK_SHADER_STAGE_VERTEX_BIT;
            case SHADER_MODULE_TYPE::FRAGMENT:
                return VK_SHADER_STAGE_FRAGMENT_BIT;
            default:
                throw new std::invalid_argument(
                    "Invalid SHADER_TYPE value used.");
        }
    };

    VulkanShader::~VulkanShader() {
        vkDestroyShaderModule(_device, _shaderModule, nullptr);
    }
}  // namespace PrestoCore
