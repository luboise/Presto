#pragma once

#include <fstream>  // IWYU pragma: export
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace Presto::Utils::File {
    // static Mesh* LoadMesh(const std::string&);

    const fs::path executableDirectory = std::filesystem::current_path();

    std::string ReadFile(const std::string& filename);
    json GetJSON(const std::string& text);
}  // namespace Presto::Utils::File