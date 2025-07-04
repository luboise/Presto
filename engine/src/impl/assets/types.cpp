module presto.assets.types;

import std;

namespace Pr {

bool Asset::ensureLoaded() {
    if (!loaded_) {
        loaded_ = this->load();

        if (!loaded_) {
            Pr::CoreLog(ERROR, "Unable to load asset \"{}\" of type {}",
                        this->name(), static_cast<Pr::uint8_t>(this->type()));
        }
    }

    return loaded_;
};

Asset::Asset(Pr::string name) : name_(std::move(name)) {};

Asset::~Asset() = default;

AssetPath::AssetPath(FilePath path) : path_(std::move(path)) {};

AssetPath::AssetPath(const char* inPath) {
    path_ = Utils::File::getFullPath(Pr::string{inPath});
}

/**
 * @brief  Return the filename without the extension
 */
Pr::string AssetPath::basename() const { return path_.stem(); }

const FilePath& AssetPath::path() const { return path_; }

bool AssetPath::valid() const {
    return fs::exists(path_) && fs::is_regular_file(path_);
};

Pr::string AssetPath::fileExtension() const { return path_.extension(); };

AssetPath::operator const FilePath&() const { return this->path(); }
AssetPath::operator FilePath() const { return this->path(); };

Pr::string AssetPath::string() const { return path_.string(); }
AssetPath::operator Pr::string() const { return string(); };

}  // namespace Pr
