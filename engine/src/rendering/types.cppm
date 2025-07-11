export module presto.internal.rendering.types;

import std;

import presto.core;
import presto.objects;
import presto.math;
import presto.utils.allocator;

import presto.rendering;

import presto.assets.material;

import presto.types.rendering;

export namespace Pr {

struct BaseAttributeTypeDetails {
    Pr::size_t subtype_size;
    Pr::size_t count;
    Pr::size_t size;
};

template <typename T>
// TODO: Fix this constraint
// requires requires { SubTypeDetails<T>::subtype; }
struct AttributeTypeDetails : BaseAttributeTypeDetails {
    using details = SubTypeDetails<T>;
    using subtype = details::subtype;

    static constexpr Pr::size_t subtype_size = sizeof(subtype);
    static constexpr Pr::size_t count{details::subtype_count};
    static constexpr Pr::size_t size = subtype_size * count;
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

}  // namespace Pr

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
