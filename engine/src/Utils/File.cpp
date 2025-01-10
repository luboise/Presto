#include "Presto/Utils/File.h"
#include "nlohmann/json_fwd.hpp"

namespace Presto::Utils {
    std::string File::ReadFile(const std::string& filename) {
        // ate <-> start at end of file
        auto filepath = executableDirectory / fs::path(filename);
        PR_CORE_TRACE(filepath.generic_string());
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            PR_CORE_ERROR("Unable to load file \"{}\"", filepath.string());
            return "";
        }

        // Get tellg position(EOF = filesize)
        auto size = static_cast<size_t>(file.tellg());
        file.seekg(0);

        std::string buffer(size, ' ');
        file.read(buffer.data(), size);
        file.close();

        return buffer;
    }

    std::vector<std::byte> File::ReadBinaryFile(const std::string& filename) {
        // ate <-> start at end of file
        auto filepath = executableDirectory / fs::path(filename);
        PR_CORE_TRACE(filepath.generic_string());
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            PR_CORE_ERROR("Unable to load file \"{}\"", filepath.string());
            return {};
        }

        // Get tellg position(EOF = filesize)
        auto size = (file.tellg());
        file.seekg(0);

        std::vector<std::byte> buffer(size);

        file.read(reinterpret_cast<char*>(buffer.data()), size);
        file.close();

        return buffer;
    };

    json File::GetJSON(const std::string& text) {
        try {
            return json::parse(text);
        } catch (...) {
            return {};
        }
    }

    json File::GetJSON(const fs::path& path) {
        return File::GetJSON(File::ReadFile(path));
    }

    fs::path File::getFullPath(const AssetPath& path) {
        return executableDirectory / path;
    };
}  // namespace Presto::Utils