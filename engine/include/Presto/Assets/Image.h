#pragma once

#include <cstddef>

#include "Presto/Types/CoreTypes.h"

namespace Presto {
struct Image {
    using image_data_t = ByteArray;

    std::size_t width;
    std::size_t height;
    image_data_t bytes;

    static constexpr auto PIXEL_BYTE_LENGTH = 4;

    // 4 bytes since assumed RGBA, 1 for each channel
    [[nodiscard]] std::size_t size() const {
        return width * height * PIXEL_BYTE_LENGTH;
    };
};
}  // namespace Presto
