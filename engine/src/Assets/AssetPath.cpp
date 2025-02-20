#include "Presto/Assets/AssetPath.h"

#include "Presto/Utils/File.h"

#include <utility>

namespace Presto {

AssetPath::AssetPath(FilePath path) : path_(std::move(path)) {};

AssetPath::AssetPath(const char* inPath) {
    path_ = Utils::File::getFullPath(Presto::string{inPath});
}

Presto::string AssetPath::basename() const { return path_.stem(); }

const FilePath& AssetPath::path() const { return path_; }

AssetPath::operator const FilePath&() const { return this->path(); }
AssetPath::operator FilePath() const { return this->path(); };

bool AssetPath::valid() const {
    return fs::exists(path_) && fs::is_regular_file(path_);
};

Presto::string AssetPath::fileExtension() const { return path_.extension(); };

AssetPath::operator Presto::string() const { return path_.string(); };

Presto::string AssetPath::string() const { return path_.string(); }
explicit AssetPath::operator Presto::string() const { return string(); };
}  // namespace Presto
