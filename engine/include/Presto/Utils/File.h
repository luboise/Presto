#pragma once

#include <fstream>  // IWYU pragma: export
#include <nlohmann/json.hpp>
#include "Presto/Core/Types.h"

using json = nlohmann::json;

namespace Presto::Utils::File {
    // static Mesh* LoadMesh(const std::string&);

    const fs::path executableDirectory = std::filesystem::current_path();

    fs::path getFullPath(const AssetPath& path);

    std::string ReadFile(const std::string& filename);
    std::vector<std::byte> ReadBinaryFile(const std::string& filename);
    json GetJSON(const std::string& text);
}  // namespace Presto::Utils::File
