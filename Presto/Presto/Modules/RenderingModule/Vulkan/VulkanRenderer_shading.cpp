#include "VulkanRenderer.h"

#include "Presto/Modules/ResourcesModule/ResourceManager.h"

namespace Presto {
    VkShaderModule VulkanRenderer::createShaderModule(
        const std::string& filepath) {
        auto code = ResourceManager::readFile(filepath);
        return createShaderModule(code);
    }

    VkShaderModule VulkanRenderer::createShaderModule(
        const std::vector<char>& code) {
        PR_ASSERT(code.size() > 0,
                  "Attempted to create a shader from an empty code vector.");

        VkShaderModuleCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;

        if (vkCreateShaderModule(_logicalDevice, &createInfo, nullptr,
                                 &shaderModule) != VK_SUCCESS) {
            std::stringstream ss;

            ss << "Failed to create shader module of size " << code.size()
               << ".";

            throw std::runtime_error(ss.str());
        }

        return shaderModule;
    }
}  // namespace Presto