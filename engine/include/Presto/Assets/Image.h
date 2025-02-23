#pragma once

#include <cstddef>

#include "Presto/Core/Types.h"

namespace Presto {
struct Image {
    std::size_t width;
    std::size_t height;
    ByteArray bytes;

    static constexpr auto PIXEL_BYTE_LENGTH = 4;

    // 4 bytes since assumed RGBA, 1 for each channel
    [[nodiscard]] std::size_t size() const {
        return width * height * PIXEL_BYTE_LENGTH;
    };
};
}  // namespace Presto
