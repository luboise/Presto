#pragma once

#include <fstream>  // IWYU pragma: export
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace fs = std::filesystem;

namespace Presto::Utils::File {
    // static Mesh* LoadMesh(const std::string&);

    const fs::path executableDirectory = std::filesystem::current_path();

    std::string ReadFile(const std::string& filename);
    static json GetJSON(const std::string& text);
}  // namespace Presto::Utils::File
