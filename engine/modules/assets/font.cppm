export module presto.assets.font;

import std;

import presto.assets.asset;
import presto.types.core;

export namespace Pr {

class FontAsset final : public Asset {
   public:
    explicit FontAsset(Pr::string name);

    ~FontAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::FONT;
    };

   private:
    bool load() override;
};

class FontSource final : public AssetSource {
   public:
    explicit FontSource(AssetPath filepath);
    ~FontSource() override;

    [[nodiscard]] Pr::Ptr<Pr::FontAsset> getFont(const Pr::string& name);
    [[nodiscard]] Pr::Ptr<Pr::FontAsset> loadFont(Pr::string fontName,
                                                  bool allowReload = true);
    void unloadFont(const Pr::string& fontName);

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
        Pr::string name;
        Pr::Ptr<Pr::FontAsset> ptr{nullptr};
    };

    LoadedFont* getLoadedFont(const Pr::string& name);
    void unloadFont(LoadedFont& font);

    std::vector<LoadedFont> fonts_;
};

}  // namespace Pr
