#pragma once

#include <nlohmann/json.hpp>
#include "Presto/Core/Types.h"

using json = nlohmann::json;

namespace Presto::Utils::File {
    // static Mesh* LoadMesh(const std::string&);

    const fs::path executableDirectory = std::filesystem::current_path();

    fs::path getFullPath(const AssetPath& path);

    std::string ReadFile(const fs::path& path);
    std::vector<std::byte> ReadBinaryFile(const fs::path& path);

    json GetJSON(const std::string& text);
    json GetJSON(const fs::path& path);
}  // namespace Presto::Utils::File
