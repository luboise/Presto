#include "Presto/Assets/ImageData.h"

namespace Presto {

#define BLACK_PIXEL std::byte{0}, std::byte{0}, std::byte{0}, std::byte{255}
#define WHITE_PIXEL \
    std::byte{255}, std::byte{255}, std::byte{255}, std::byte{255}

const ByteArray default_texture_bytes{BLACK_PIXEL, WHITE_PIXEL, WHITE_PIXEL,
                                      BLACK_PIXEL};

static const Presto::ImageData DEFAULT_TEXTURE_DATA{
    .width = 2,
    .height = 2,
    .bytes{default_texture_bytes.begin(), default_texture_bytes.end()}};

#undef BLACK_PIXEL
#undef WHITE_PIXEL

}  // namespace Presto
