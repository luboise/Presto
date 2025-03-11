#pragma once

#include "Presto/Core/Constants.h"
#include "Presto/Rendering/VertexTypes.h"
#include "Presto/Types/CoreTypes.h"

namespace Presto {

struct GlobalUniforms {
    Presto::mat4 view;
    Presto::mat4 projection;
};

struct ObjectUniforms {
    Presto::mat4 transform;
};

struct CanvasItemAttributes {
    Presto::vec2 position{0, 0};
    Presto::vec2 offset{0, 0};
    Presto::float32_t scale{1};
};

struct DrawInfo {
    std::size_t vertex_count = 0;

    // Vulkan uses signed int for the offset
    int32_t vertex_offset = 0;

    std::size_t index_count = 0;
    uint32_t index_offset = 0;
};

using Index = uint32_t;

struct Vertex;

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

enum class ShaderStage { VERTEX, FRAGMENT };

enum class MeshDrawMode : Presto::uint8_t {
    POINTS,
    LINES,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP
};

struct MeshData {
    pipeline_id_t pipeline_id{PR_PIPELINE_NONE};

    MeshDrawMode draw_mode{MeshDrawMode::TRIANGLES};
    AnyVertexList vertices;
    IndexList indices;
    // [[nodiscard]] BoundingBox getBoundingBox() const;
};

using CanvasUnits = Presto::float32_t;

constexpr CanvasUnits PR_CANVAS_MIN_VALUE = -1;
constexpr CanvasUnits PR_CANVAS_MAX_VALUE = 1;

using CanvasPosition = Presto::vec2;

/** @brief Clamps a canvas position to be within the canvas bounds.
 *  @see PR_CANVAS_MIN_VALUE and PR_CANVAS_MAX_VALUE
 */
CanvasPosition Clamped(const CanvasPosition& pos);

}  // namespace Presto
