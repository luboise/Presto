export module presto.assets.font;
import presto.assets.types;

export namespace Presto {
class FontAsset final : public Asset {
    friend class RenderingManager;

   public:
    explicit FontAsset(Presto::string name);

    ~FontAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::FONT;
    };

   private:
    bool load() override;
};

struct ImportedFontData;

class FontSource final : public AssetSource {
   public:
    explicit FontSource(AssetPath filepath);
    ~FontSource() override;

    [[nodiscard]] FontPtr getFont(const Presto::string& name);
    [[nodiscard]] FontPtr loadFont(Presto::string fontName,
                                   bool allowReload = true);
    void unloadFont(const Presto::string& fontName);

    void reloadFile();
    void unload() override;

    FontSource(const FontSource&) = delete;
    FontSource(FontSource&&) = delete;
    FontSource& operator=(const FontSource&) = delete;
    FontSource& operator=(FontSource&&) = delete;

   private:
    // void updateFont();

    // Allocated<ImportedFontData> importData_;

    struct LoadedFont {
        Presto::string name;
        FontPtr ptr{nullptr};
    };

    LoadedFont* getLoadedFont(const Presto::string& name);
    void unloadFont(LoadedFont& font);

    std::vector<LoadedFont> fonts_;
};
}  // namespace Presto
