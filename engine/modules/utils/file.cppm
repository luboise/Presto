module;

#include <nlohmann/json.hpp>

export module presto.utils:file;

import presto.core.types;
import presto.assets.types;

using json = nlohmann::json;

export namespace Pr::Utils::File {
// static Mesh* LoadMesh(const Pr::string&);

const FilePath executableDirectory{std::filesystem::current_path()};

FilePath getFullPath(const Pr::string& path);

Pr::string ReadAssetFile(const AssetArg&);

Pr::string ReadFile(const FilePath& path);
ByteArray ReadBinaryFile(const FilePath& path);

json GetJSON(const Pr::string& text);
json GetJSON(const fs::path& path);

}  // namespace Pr::Utils::File
