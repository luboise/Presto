#include <nlohmann/json/hpp>

#include "presto/assets/types.h"
#include "presto/core/types.h"

using json = nlohmann::json;

namespace Presto::Utils::File {
// static Mesh* LoadMesh(const Presto::string&);

const FilePath executableDirectory{std::filesystem::current_path()};

FilePath getFullPath(const Presto::string& path);

Presto::string ReadAssetFile(const AssetArg&);

Presto::string ReadFile(const FilePath& path);
ByteArray ReadBinaryFile(const FilePath& path);

json GetJSON(const Presto::string& text);
json GetJSON(const fs::path& path);
}  // namespace Presto::Utils::File
