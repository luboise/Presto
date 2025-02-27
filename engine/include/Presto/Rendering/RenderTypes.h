#pragma once

#include "Presto/Core/Constants.h"
#include "Presto/Types/CoreTypes.h"

#include "Presto/Rendering/BoundingBox.h"

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

constexpr PR_NUMERIC_ID UNREGISTERED_RENDER_DATA_ID = -1;

using renderer_mesh_id_t = PR_NUMERIC_ID;
using renderer_texture_id_t = PR_NUMERIC_ID;

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

struct MeshData {
    int draw_mode;
    VertexList vertices;
    IndexList indices;

    static MeshData from(const RawMeshData&);
    [[nodiscard]] BoundingBox getBoundingBox() const;
};

enum class ShaderStage { VERTEX, FRAGMENT };

// TODO: Make this more robust and give it more values, such as attributes. At
// the moment, the attributes can be retrieved using the pipeline handle using
// getPipelineStructure(id)
struct ProcessedVertexData {
    pipeline_id_t pipeline_id;

    // AttributeSet attributes;
    Presto::size_t vertex_count;
    ByteArray data;
};

}  // namespace Presto
