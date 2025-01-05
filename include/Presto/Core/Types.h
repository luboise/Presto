#pragma once

#include <filesystem>
#include <vector>

#include "Presto/Math.h"

namespace fs = std::filesystem;

namespace Presto {
    struct Vertex {
        vec3 pos;
        vec3 colour;
    };

    using Index = uint32_t;

    using VertexList = std::vector<Vertex>;
    using IndexList = std::vector<Index>;

    using PR_STRING_ID = std::string;

    using AssetPath = fs::path;

}  // namespace Presto
