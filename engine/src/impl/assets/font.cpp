module;

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

module presto.assets.font;

import std;

import presto.utils;
import presto.core;
import presto.core.assert;

namespace Pr {

FontAsset::FontAsset(Pr::string name) : Asset(std::move(name)) {}

// TODO: Implement
bool FontAsset::load() { return true; };

FontSource::FontSource(AssetPath filepath)
    : AssetSource(std::move(filepath), true) {
    reloadFile();
};

void FontSource::reloadFile() {
    stbtt_fontinfo info{};
    ByteArray font_bytes =
        Utils::File::ReadBinaryFile("assets/fonts/Inter.ttc");

    Pr::Assert(font_bytes.size() > 0, "Font couldn't be loaded.");

    std::span<unsigned char> span{
        reinterpret_cast<unsigned char*>(font_bytes.data()), font_bytes.size()};

    Pr::Assert(stbtt_GetNumberOfFonts(span.data()) > 0,
               "No fonts found in Inter.ttc.");

    int index{stbtt_GetFontOffsetForIndex(span.data(), 0)};

    stbtt_InitFont(&info, &span[index], 0);

    //   "Load" a font file from a memory buffer (you have to keep the buffer
    //   loaded)
    //           stbtt_InitFont()
    //           stbtt_GetFontOffsetForIndex()        -- indexing for TTC font
    //           collections stbtt_GetNumberOfFonts()             -- number of
    //           fonts for TTC font collections
    //
    //   Render a unicode codepoint to a bitmap
    //           stbtt_GetCodepointBitmap()           -- allocates and returns a
    //           bitmap stbtt_MakeCodepointBitmap()          -- renders into
    //           bitmap you provide stbtt_GetCodepointBitmapBox()        -- how
    //           big the bitmap must be
    //
    //   Character advance/positioning
    //           stbtt_GetCodepointHMetrics()
    //           stbtt_GetFontVMetrics()
    //           stbtt_GetFontVMetricsOS2()
    //           stbtt_GetCodepointKernAdvance()
    Pr::CoreLog(TRACE, true);
}

// TODO: Move this to the base AssetSource
FontSource::~FontSource() {
    if (loaded()) {
        unload();
    }
};

// TODO: Implement
void FontSource::unload() {};

}  // namespace Pr
