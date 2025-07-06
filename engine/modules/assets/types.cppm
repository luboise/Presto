export module presto.assets.types;

import std;

import presto.core;
import presto.core.concepts;

export namespace Pr {

using asset_name_t = Pr::string;
enum class AssetType : Pr::uint8_t {
    MESH,
    MODEL,
    MATERIAL_DEFINITION,
    IMAGE,
    FONT
};

class AssetPath {
   public:
    AssetPath(const char*);  // Ignore warning, the conversion is intended

    explicit AssetPath(FilePath path);

    [[nodiscard]] Pr::string basename() const;

    [[nodiscard]] Pr::string fileExtension() const;

    [[nodiscard]] const FilePath& path() const;

    operator const FilePath&() const;
    explicit operator FilePath() const;

    [[nodiscard]] Pr::string string() const;
    explicit operator Pr::string() const;

    [[nodiscard]] bool valid() const;

   private:
    FilePath path_;
};

using AssetArg = AssetPath;

class Asset {
   public:
    explicit Asset(asset_name_t name = PR_ANY_NAME);
    virtual ~Asset();

    [[nodiscard]] constexpr virtual AssetType type() const = 0;

    bool ensureLoaded();

    [[nodiscard]] bool loaded() const { return loaded_; }

    [[nodiscard]] asset_name_t name() const { return name_; }

    template <DerivedFrom<Asset> Subclass>
    Subclass* as() {
        return dynamic_cast<Subclass*>(this);
    }

    Asset(const Asset&) = delete;
    Asset(Asset&&) = delete;
    Asset& operator=(const Asset&) = delete;
    Asset& operator=(Asset&&) = delete;

   private:
    [[nodiscard]] virtual bool load() = 0;

    asset_name_t name_;

    bool loaded_{false};
};

template <DerivedFrom<Asset> T>
using AssetPtr = Ptr<T>;

class AssetSource {
   public:
    virtual ~AssetSource() { this->ensureUnloaded(); }

    virtual void unload() = 0;

    [[nodiscard]] bool loaded() const { return loaded_; }
    [[nodiscard]] AssetPath path() const { return path_; }

    virtual void setPath(AssetPath path) {
        this->ensureUnloaded();
        path_ = std::move(path);
    };

    AssetSource(const AssetSource&) = default;
    AssetSource(AssetSource&&) = delete;
    AssetSource& operator=(const AssetSource&) = default;
    AssetSource& operator=(AssetSource&&) = delete;

   protected:
    AssetSource() : path_(""), loaded_(false) {}

    void ensureUnloaded() {
        // if (loaded_) {
        // this->unload();
        // }
    }

    explicit AssetSource(AssetPath path, bool loaded)
        : path_(std::move(path)), loaded_(loaded) {}

   private:
    AssetPath path_;
    bool loaded_;
};

}  // namespace Pr
