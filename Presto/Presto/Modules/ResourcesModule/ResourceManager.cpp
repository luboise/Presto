#include "ResourceManager.h"

#include <fstream>
#include <vector>
#include <filesystem>

namespace Presto {
    ResourceManager::ResourceManager() { this->Init(); }

    ResourceManager::~ResourceManager() {}

    std::vector<char> ResourceManager::readFile(const std::string& filename) {
        // ate <-> start at end of file
        PR_CORE_TRACE(std::filesystem::current_path().string());
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            PR_CORE_ERROR("Unable to load file \"{}\"", filename);
            return std::vector<char>(0);
        }

        // Get tellg position(EOF = filesize)
        size_t fileSize = (size_t)file.tellg();
        file.seekg(0);

        std::vector<char> buffer(fileSize);
        file.read(buffer.data(), fileSize);

        file.close();
        return buffer;
    }

    void ResourceManager::Init() {
        this->_initialised = true;
        return;
    }
    void ResourceManager::Shutdown() {
        this->_initialised = false;
        return;
    }
}  // namespace Presto