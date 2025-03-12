#include "Presto/Utils/File.h"

#include "Presto/Types/CoreTypes.h"

namespace Presto::Utils {

std::string File::ReadFile(const FilePath& path) {
    // ate <-> start at end of file
    // auto filepath = executableDirectory / fs::path(path);
    PR_CORE_TRACE("Reading file at path {}", path.generic_string());
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        PR_CORE_ERROR("Unable to load file \"{}\"", path.string());
        return "";
    }

    // Get tellg position(EOF = filesize)
    const auto size = static_cast<size_t>(file.tellg());
    file.seekg(0);

    std::string buffer(size, ' ');
    file.read(buffer.data(), size);
    file.close();

    return buffer;
}

ByteArray File::ReadBinaryFile(const FilePath& path) {
    // ate <-> start at end of file
    // auto filepath = executableDirectory / fs::path(filename);
    PR_CORE_TRACE(path.generic_string());
    std::ifstream file(path, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        PR_CORE_ERROR("Unable to load file \"{}\"", path.string());
        return {};
    }

    // Get tellg position(EOF = filesize)
    const auto size = (file.tellg());
    file.seekg(0);

    ByteArray buffer(size);

    file.read(reinterpret_cast<char*>(buffer.data()), size);
    file.close();

    return buffer;
};

// TODO: Implement proper error handling
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

fs::path File::getFullPath(const Presto::string& path) {
    return executableDirectory / path;
};

std::string File::ReadAssetFile(const AssetArg& path) {
    return ReadFile(path.string());
};

}  // namespace Presto::Utils
