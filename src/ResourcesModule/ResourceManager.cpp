#include "ResourceManager.h"

#include <filesystem>
#include <fstream>
#include <vector>
namespace fs = std::filesystem;

namespace Presto {
    // const fs::path executableDirectory =
    // fs::absolute(fs::path(argv[0])).parent_path();

    // TODO: Fix this to be based on the executable path rather than the
    // directory which the program was called from
    const fs::path executableDirectory = std::filesystem::current_path();

    ResourceManager::ResourceManager() { this->Init(); }

    ResourceManager::~ResourceManager() { this->Shutdown(); }

    std::vector<char> ResourceManager::readFile(const std::string& filename) {
        // ate <-> start at end of file
        auto filepath = executableDirectory / fs::path(filename);
        PR_CORE_TRACE(filepath.generic_string());
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            PR_CORE_ERROR("Unable to load file \"{}\"", filepath.string());
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

    void ResourceManager::F_INIT() {}
    void ResourceManager::F_UPDATE() {}
    void ResourceManager::F_SHUTDOWN() {}
}  // namespace Presto
