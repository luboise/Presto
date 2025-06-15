export module presto.assets.types;

import presto.core;

export namespace Presto {

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

}  // namespace Presto
