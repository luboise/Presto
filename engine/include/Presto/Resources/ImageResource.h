#pragma once

#include "Presto/Core/Types.h"

// #include "Presto/Rendering/Renderer.h"

namespace Presto {
    struct ImageResource {
        friend class ResourceManager;

        PR_STRING_ID name;

        std::size_t width;
        std::size_t height;
        std::vector<uint8_t> data;

        // 4 bytes since assumed RGBA, 1 for each channel
        [[nodiscard]] std::size_t getByteCount() const {
            return width * height * 4;
        };

        /*
render_data_id_t renderId_ = UNREGISTERED_RENDER_DATA_ID;

[[nodiscard]] bool isRegistered() const {
    return renderId_ != UNREGISTERED_RENDER_DATA_ID;
}
        */
    };
}  // namespace Presto
