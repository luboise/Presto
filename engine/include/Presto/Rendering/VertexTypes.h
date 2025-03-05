#pragma once

#include <variant>

namespace Presto {

struct Vertex3D {
    Presto::vec3 vertexPosition;
    Presto::vec3 colour;
    Presto::vec3 normal;
    Presto::vec2 tex_coords;
};

struct VertexUI {
    Presto::vec2 vertexPosition;
    Presto::vec3 colour;
    Presto::vec2 texcoords;
};

using AnyVertexType = std::variant<Vertex3D, VertexUI>;
using AnyVertexList =
    std::variant<std::vector<Vertex3D>, std::vector<VertexUI>>;

}  // namespace Presto
