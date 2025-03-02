#pragma once

#include "Presto/Core/Constants.h"
#include "Presto/Types/CoreTypes.h"
#include "VertexTypes.h"

namespace Presto {

struct GlobalUniforms {
    Presto::mat4 view;
    Presto::mat4 projection;
};

struct ObjectUniforms {
    Presto::mat4 transform;
};

struct DrawInfo {
    std::size_t vertex_count = 0;

    // Vulkan uses signed int for the offset
    int32_t vertex_offset = 0;

    std::size_t index_count = 0;
    uint32_t index_offset = 0;
};

using Index = uint32_t;

class Vertex;

using VertexList = std::vector<Vertex>;
using IndexList = std::vector<Index>;

struct RawMeshData {
    using PositionType = vec3;
    using NormalType = vec3;
    using TexCoordsType = vec2;
    using IndexType = Presto::uint32_t;

    std::vector<PositionType> positions;
    std::vector<NormalType> normals;
    std::vector<TexCoordsType> tex_coords;

    // size_t index_count;
    std::vector<IndexType> indices;

    int draw_mode{};
};

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

/*
layout(location = 0) in vec3 a_vertexPosition;
layout(location = 1) in vec3 a_colour;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec2 a_texcoords;
*/

enum class ShaderStage { VERTEX, FRAGMENT };

enum class MeshDrawMode : Presto::uint8_t {
    POINTS,
    LINES,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP
};

// TODO: Make this more robust and give it more values, such as attributes. At
// the moment, the attributes can be retrieved using the pipeline handle using
// getPipelineStructure(id)
struct ProcessedMeshVertices {
    pipeline_id_t pipeline_id;

    // AttributeSet attributes;
    Presto::size_t vertex_count;

    std::vector<Vertex3D> vertices;
};

struct MeshData {
    MeshDrawMode draw_mode;
    ProcessedMeshVertices vertex_data;
    IndexList indices;
    // [[nodiscard]] BoundingBox getBoundingBox() const;
};

}  // namespace Presto
