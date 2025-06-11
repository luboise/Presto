#include "Presto/Assets/FontAsset.h"
#include "Presto/Utils/File.h"

#include <stb_truetype.h>

namespace Presto {

#define STB_TRUETYPE_IMPLEMENTATION

FontAsset::FontAsset(PR_STRING_ID name) : Asset(std::move(name)) {
    stbtt_fontinfo info{};
    ByteArray font_bytes =
        Utils::File::ReadBinaryFile("assets/fonts/inter.ttc");

    std::span<unsigned char> span{
        reinterpret_cast<unsigned char*>(font_bytes.data()), font_bytes.size()};
    stbtt_InitFont(&info, span.data(), 0);

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
}

}  // namespace Presto
