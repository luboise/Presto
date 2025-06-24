#include "internal/rendering/draw_types.h"
#include "presto/assets/material_types.h"  // IWYU pragma: export
#include "presto/core.h"
#include "presto/math/transform_data.h"
#include "presto/objects.h"
#include "presto/utils/allocator.h"

namespace Pr {
struct Buffer;
struct Vertex3D;
struct VertexUI;

class Pipeline;

struct Vertex;
using VertexList = std::vector<Vertex>;

using Index = uint32_t;
using IndexList = std::vector<Index>;

using AnyVertexType = std::variant<Vertex3D, VertexUI>;
using AnyVertexList =
    std::variant<std::vector<Vertex3D>, std::vector<VertexUI>>;

struct CanvasDrawDetails {
    mesh_registration_id_t mesh;
    TransformData transform;
};

struct ImportedAttributeList;
struct MeshData {
    pipeline_id_t pipeline_id{PR_PIPELINE_ANY};

    MeshDrawMode draw_mode{MeshDrawMode::TRIANGLES};
    AnyVertexList vertices;
    IndexList indices;
    // [[nodiscard]] BoundingBox getBoundingBox() const;
    //

    void setVertices(const ImportedAttributeList& attributes);
};

struct GlobalUniforms {
    Pr::mat4 view;
    Pr::mat4 projection;
};

struct ObjectUniforms {
    Pr::mat4 transform;
};

struct DrawInfo {
    Pr::size_t vertex_count = 0;

    // Vulkan uses signed int for the offset
    int32_t vertex_offset = 0;

    Pr::size_t index_count = 0;
    uint32_t index_offset = 0;
};

struct RawMeshData {
    using PositionType = vec3;
    using NormalType = vec3;
    using TexCoordsType = vec2;
    using IndexType = Pr::uint32_t;

    std::vector<PositionType> positions;
    std::vector<NormalType> normals;
    std::vector<TexCoordsType> tex_coords;

    // size_t index_count;
    std::vector<IndexType> indices;

    int draw_mode{};
};

}  // namespace Pr
