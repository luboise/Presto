export module presto.assets.types;

import presto.core;

export namespace Presto {

using asset_name_t = Presto::string;
enum class AssetType : std::uint8_t {
    MESH,
    MODEL,
    MATERIAL_DEFINITION,
    IMAGE,
    FONT
};

using AssetArg = AssetPath;

class MaterialAsset;
using MaterialDefinitionPtr = Ptr<MaterialAsset>;
class ImageAsset;
using ImagePtr = Ptr<ImageAsset>;
class ModelAsset;
using ModelPtr = Ptr<ModelAsset>;
class FontAsset;
using FontPtr = Ptr<FontAsset>;

class Mesh;
using MeshPtr = Ptr<Mesh>;

class AssetPath {
   public:
    AssetPath(const char*);  // Ignore warning, the conversion is intended

    explicit AssetPath(FilePath path);

    [[nodiscard]] Presto::string basename() const;

    [[nodiscard]] Presto::string fileExtension() const;

    [[nodiscard]] const FilePath& path() const;

    operator const FilePath&() const;
    explicit operator FilePath() const;

    [[nodiscard]] Presto::string string() const;
    explicit operator Presto::string() const;

    [[nodiscard]] bool valid() const;

   private:
    FilePath path_;
};

class Asset {
    friend class AssetManager;

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

export namespace Presto {

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

}  // namespace Presto
}  // namespace Presto
