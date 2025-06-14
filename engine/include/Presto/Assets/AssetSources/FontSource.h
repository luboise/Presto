#pragma once

#include "AssetSource.h"

#include "Presto/Types/AssetTypes.h"
#include "Presto/Types/CoreTypes.h"

namespace Presto {

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
