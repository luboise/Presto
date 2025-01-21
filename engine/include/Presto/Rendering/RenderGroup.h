#pragma once

#include "Presto/Core/Types.h"
#include "Presto/Resources/MaterialResource.h"

namespace Presto {
    struct RenderData {
        int draw_mode;
        VertexList vertices;
        IndexList indices;

        MaterialResource* material;
    };

    // Done as a struct in case more details need to be added later (eg. static
    // or dynamic, name, etc)
    struct RenderGroup {
        std::vector<RenderData> render_list;
    };
}  // namespace Presto
