#pragma once

#include <filesystem>
#include <vector>

#include <glm/glm.hpp>
#include "glm/fwd.hpp"

namespace fs = std::filesystem;

namespace Presto {

using float32_t = glm::float32_t;
using size_t = std::size_t;

using int8_t = std::int8_t;
using int32_t = std::int32_t;

using uint8_t = std::uint8_t;
using uint32_t = std::uint32_t;

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

using mat4 = glm::mat4;

/*struct Vertex {*/
/*    vec3 position;*/
/*    vec3 colour;*/
/*    vec3 normal;*/
/*    vec2 tex_coords;*/
/*};*/

class Vertex {
   public:
    template <typename T>
    T getAttribute() const;

   private:
};

using ByteArray = std::vector<std::byte>;

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
using Allocated = std::unique_ptr<T>;

template <typename T>
using Ptr = std::shared_ptr<T>;

template <typename T>
using Ref = std::weak_ptr<T>;
}  // namespace Presto
