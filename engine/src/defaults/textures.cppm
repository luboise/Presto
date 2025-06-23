#include "presto/assets/image.h"
#include "presto/core/types.h"

export {
#define BLACK_PIXEL std::byte{0}, std::byte{0}, std::byte{0}, std::byte{255}
#define WHITE_PIXEL \
    std::byte{255}, std::byte{255}, std::byte{255}, std::byte{255}

    const Presto::ByteArray default_texture_bytes{BLACK_PIXEL, WHITE_PIXEL,
                                                  WHITE_PIXEL, BLACK_PIXEL};

    const Presto::ImageData DEFAULT_TEXTURE_DATA{
        .width = 2,
        .height = 2,
        .bytes{default_texture_bytes.begin(), default_texture_bytes.end()}};

#undef BLACK_PIXEL
#undef WHITE_PIXEL
}
