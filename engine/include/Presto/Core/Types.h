#pragma once

#include <filesystem>
#include <vector>

#include <glm/glm.hpp>

namespace fs = std::filesystem;

namespace Presto {
    using mat4 = glm::mat4;
    using vec2 = glm::vec2;
    using vec3 = glm::vec3;

    struct Vertex {
        vec3 position;
        vec3 colour;
        vec3 normal;
        vec2 tex_coords;
    };

    using Index = uint32_t;

    using VertexList = std::vector<Vertex>;
    using IndexList = std::vector<Index>;

    using PR_STRING_ID = std::string;

    using AssetPath = fs::path;
    using asset_name_t = PR_STRING_ID;

    struct VisualExtents {
        std::uint16_t width;
        std::uint16_t height;
    };

    template <typename T>
    using Ptr = std::shared_ptr<T>;

    template <typename T>
    using Ref = std::weak_ptr<T>;
}  // namespace Presto
