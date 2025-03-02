#pragma once

namespace Presto {

struct Vertex3D {
    Presto::vec3 vertexPosition;
    Presto::vec3 colour;
    Presto::vec3 normal;
    Presto::vec2 tex_coords;
};

}  // namespace Presto
