export module presto.rendering.vertices;

import std;

import presto.assets.importing;

import presto.types.core;

import presto.core.constants;
import presto.types.rendering;

export namespace Pr {

struct Vertex {};
using VertexList = std::vector<Vertex>;

struct Vertex3D : public Vertex {
    Pr::vec3 vertexPosition;
    Pr::vec3 colour = {1, 1, 1};
    Pr::vec3 normal;
    Pr::vec2 tex_coords;
};

struct VertexUI : public Vertex {
    Pr::vec2 vertexPosition;
    Pr::vec4 colour;
    Pr::vec2 tex_coords;
};

using AnyVertexType = std::variant<Vertex3D, VertexUI>;
using AnyVertexList =
    std::variant<std::vector<Vertex3D>, std::vector<VertexUI>>;

struct MeshData {
    pipeline_id_t pipeline_id{PR_PIPELINE_ANY};

    MeshDrawMode draw_mode{MeshDrawMode::TRIANGLES};
    AnyVertexList vertices;
    IndexList indices;
    // [[nodiscard]] BoundingBox getBoundingBox() const;
    //

    void setVertices(const ImportedAttributeList& attributes);
};

}  // namespace Pr
