#pragma once

#include <nlohmann/json.hpp>

#include "Presto/Core/Types.h"

using json = nlohmann::json;

namespace Presto::Utils::File {
// static Mesh* LoadMesh(const std::string&);

const FilePath executableDirectory{std::filesystem::current_path()};

FilePath getFullPath(const Presto::string& path);

std::string ReadFile(const FilePath& path);
ByteArray ReadBinaryFile(const FilePath& path);

json GetJSON(const std::string& text);
json GetJSON(const fs::path& path);
}  // namespace Presto::Utils::File
