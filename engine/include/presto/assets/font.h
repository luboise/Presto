#pragma once

#include "presto/assets/asset.h"
#include "presto/core/types.h"
#include "presto/handles/asset_handles.h"

namespace Pr {

class FontAsset final : public Asset {
    friend class RenderingManager;

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

    [[nodiscard]] FontPtr getFont(const Pr::string& name);
    [[nodiscard]] FontPtr loadFont(Pr::string fontName,
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
        FontPtr ptr{nullptr};
    };

    LoadedFont* getLoadedFont(const Pr::string& name);
    void unloadFont(LoadedFont& font);

    std::vector<LoadedFont> fonts_;
};

}  // namespace Pr
