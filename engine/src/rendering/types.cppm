export module presto.internal.rendering:types;

import std;

import presto.core;
import presto.objects;
import presto.math;
import presto.utils;

import presto.assets.material;

import presto.types.rendering;

export namespace Pr {

struct CanvasDrawDetails {
    mesh_registration_id_t mesh;
    TransformData transform;
};

struct MeshRegistrationData {
    // The ID of the mesh's place in the RenderingManager
    mesh_registration_id_t render_manager_id;
    Allocated<Buffer> vertices;
    Allocated<Buffer> indices;

    Pr::size_t index_draw_count{0};

    MeshDrawMode draw_mode{MeshDrawMode::TRIANGLES};
    // The ID that points to the internal details in the renderer
    mesh_context_id_t context_id{-1U};
};

struct MeshData {
    pipeline_id_t pipeline_id{PR_PIPELINE_ANY};

    MeshDrawMode draw_mode{MeshDrawMode::TRIANGLES};
    AnyVertexList vertices;
    IndexList indices;
    // [[nodiscard]] BoundingBox getBoundingBox() const;
    //

    void setVertices(const ImportedAttributeList& attributes);
};

struct AllocatedPipeline {
    pipeline_id_t id;
    Allocated<Pipeline> pipeline;
    Pr::Ptr<Pr::MaterialInstance> default_material{nullptr};
};

using pipeline_allocator_t = Allocator<pipeline_id_t, AllocatedPipeline>;
using mesh_allocator_t =
    Allocator<mesh_registration_id_t, MeshRegistrationData>;

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

enum class ShaderStage { VERTEX, FRAGMENT };

BaseAttributeTypeDetails getShaderTypeDetails(ShaderDataType type);

Pr::BaseAttributeTypeDetails Pr::getShaderTypeDetails(ShaderDataType type) {
    using namespace Pr;

#define SWITCH_CASE(type) \
    case type:            \
        return AttributeTypeDetails<ShaderImportTypeOf<(type)>>();
    switch (type) {
        SWITCH_CASE(ShaderDataType::FLOAT);
        SWITCH_CASE(ShaderDataType::SHORT);
        SWITCH_CASE(ShaderDataType::USHORT);
        SWITCH_CASE(ShaderDataType::INT);
        SWITCH_CASE(ShaderDataType::UINT);
        SWITCH_CASE(ShaderDataType::DOUBLE);
        SWITCH_CASE(ShaderDataType::VEC2);
        SWITCH_CASE(ShaderDataType::DVEC2);
        SWITCH_CASE(ShaderDataType::VEC3);
        SWITCH_CASE(ShaderDataType::DVEC3);
        SWITCH_CASE(ShaderDataType::VEC4);
        SWITCH_CASE(ShaderDataType::DVEC4);
        SWITCH_CASE(ShaderDataType::MAT3);
        SWITCH_CASE(ShaderDataType::DMAT3);
        SWITCH_CASE(ShaderDataType::MAT4);
        SWITCH_CASE(ShaderDataType::DMAT4);
        break;
    };

#undef SWITCH_CASE

    return {};
}

}  // namespace Pr
