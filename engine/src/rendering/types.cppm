export module presto.internal.rendering:types;

import presto.core;

export namespace Presto {

struct CanvasDrawDetails {
    mesh_registration_id_t mesh;
    TransformData transform;
};

struct MeshRegistrationData {
    // The ID of the mesh's place in the RenderingManager
    mesh_registration_id_t render_manager_id;
    Allocated<Buffer> vertices;
    Allocated<Buffer> indices;

    Presto::size_t index_draw_count{0};

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

struct MeshDraw {
    const MeshPtr mesh;
    const MaterialPtr material;
};

struct AllocatedPipeline {
    pipeline_id_t id;
    Allocated<Pipeline> pipeline;
    MaterialPtr default_material{nullptr};
};

using pipeline_allocator_t = Allocator<pipeline_id_t, AllocatedPipeline>;
using mesh_allocator_t =
    Allocator<mesh_registration_id_t, MeshRegistrationData>;

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

// Shader types
template <ShaderDataType T>
struct ShaderDataTypeTraits {
    static_assert(false, "No shader data type details struct implemented.");
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::SHORT> {
    using ImportType = Presto::int16_t;
    static constexpr Presto::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::USHORT> {
    using ImportType = Presto::uint16_t;
    static constexpr Presto::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::INT> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::UINT> {
    using ImportType = Presto::uint16_t;
    static constexpr Presto::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::FLOAT> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{1};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DOUBLE> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{1};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC2> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{2};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DVEC2> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{2};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC3> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{3};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DVEC3> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{3};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::VEC4> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{4};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DVEC4> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{4};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::MAT3> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{12};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DMAT3> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{12};
};

template <>
struct ShaderDataTypeTraits<ShaderDataType::MAT4> {
    using ImportType = Presto::float32_t;
    static constexpr Presto::size_t subtype_count{16};
};
template <>
struct ShaderDataTypeTraits<ShaderDataType::DMAT4> {
    using ImportType = Presto::float64_t;
    static constexpr Presto::size_t subtype_count{16};
};

template <ShaderDataType T>
// requires requires { ShaderDataTypeTraits<T>::ImportType; }
using ShaderImportTypeOf = ShaderDataTypeTraits<T>::ImportType;

constexpr Presto::size_t SizeOfShaderType(ShaderDataType type) {
#define SWITCH_CASE(type)                                         \
    case type:                                                    \
        return sizeof(ShaderDataTypeTraits<(type)>::ImportType) * \
               ShaderDataTypeTraits<(type)>::subtype_count;

    switch (type) {
        SWITCH_CASE(ShaderDataType::SHORT);
        SWITCH_CASE(ShaderDataType::USHORT);
        SWITCH_CASE(ShaderDataType::INT);
        SWITCH_CASE(ShaderDataType::UINT);

        SWITCH_CASE(ShaderDataType::FLOAT);
        SWITCH_CASE(ShaderDataType::DOUBLE);
        SWITCH_CASE(ShaderDataType::VEC3);

        SWITCH_CASE(ShaderDataType::VEC4);
        SWITCH_CASE(ShaderDataType::MAT3);

        SWITCH_CASE(ShaderDataType::MAT4);

        SWITCH_CASE(ShaderDataType::VEC2);
        SWITCH_CASE(ShaderDataType::DVEC2);
        SWITCH_CASE(ShaderDataType::DVEC3);
        SWITCH_CASE(ShaderDataType::DVEC4);
        SWITCH_CASE(ShaderDataType::DMAT3);
        SWITCH_CASE(ShaderDataType::DMAT4);

        default: {
            PR_ERROR("No size available.");
            return 0;
        }
    }
#undef SWITCH_CASE
}

BaseAttributeTypeDetails getShaderTypeDetails(ShaderDataType type);

Presto::BaseAttributeTypeDetails Presto::getShaderTypeDetails(
    ShaderDataType type) {
    using namespace Presto;

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

// Uniform variable types

enum class UniformVariableType : Presto::uint8_t {
    INT = 1,
    UINT = 2,
    FLOAT = 3,
    VEC2 = 4,
    VEC3 = 5,
    VEC4 = 6,
    MAT3 = 7,
    MAT4 = 8,
    TEXTURE = 9,
};

template <UniformVariableType T>
struct UniformVariableTypeTraits {
    static_assert(false, "No type trait instantiation defined.");
};

template <>
struct UniformVariableTypeTraits<UniformVariableType::FLOAT> {
    using ImportType = Presto::float32_t;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::INT> {
    using ImportType = Presto::int32_t;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::UINT> {
    using ImportType = Presto::uint32_t;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::VEC2> {
    using ImportType = Presto::vec2;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::VEC3> {
    using ImportType = Presto::vec3;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::VEC4> {
    using ImportType = Presto::vec4;
    using GPUType = ImportType;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::TEXTURE> {
    // Imported as index into the array of imported textures
    using ImportType = Presto::uint8_t;
    using GPUType = Presto::int32_t;
};
template <>
struct UniformVariableTypeTraits<UniformVariableType::MAT4> {
    using ImportType = Presto::mat4;
    using GPUType = ImportType;
};

template <UniformVariableType T>
// Enforce specialisation
    requires requires { typename UniformVariableTypeTraits<T>; }
using ImportTypeOf = UniformVariableTypeTraits<T>::ImportType;

template <UniformVariableType T>
    requires requires { typename UniformVariableTypeTraits<T>; }
using GPUTypeOf = UniformVariableTypeTraits<T>::GPUType;

constexpr Presto::size_t SizeOfType(UniformVariableType type) noexcept {
#define SWITCH_CASE(type) \
    case type:            \
        return sizeof(UniformVariableTypeTraits<type>::GPUType);

    switch (type) {
        SWITCH_CASE(UniformVariableType::INT);
        SWITCH_CASE(UniformVariableType::UINT);
        SWITCH_CASE(UniformVariableType::FLOAT);
        SWITCH_CASE(UniformVariableType::VEC2);
        SWITCH_CASE(UniformVariableType::VEC3);
        SWITCH_CASE(UniformVariableType::VEC4);
        SWITCH_CASE(UniformVariableType::MAT4);
        SWITCH_CASE(UniformVariableType::TEXTURE);
        default: {
            return 0;
        }
    }
#undef SWITCH_CASE
};

using uniform_name_t = Presto::string;

using uniform_index_t = Presto::uint8_t;
constexpr uniform_index_t PR_INVALID_UNIFORM = -1;

struct UniformBinding {
    enum : Presto::uint8_t { SINGLE, BLOCK };

    Presto::uint8_t bind_type;
    UniformVariableType data_type;
    uniform_name_t name;

    union {
        Presto::uint32_t location;
        Presto::uint32_t offset;
    };

    [[nodiscard]] Presto::size_t size() const;
};

struct UniformBlock {
    uniform_index_t bind_point;
    uniform_name_t name;

    std::vector<UniformBinding> bindings;

    [[nodiscard]] Presto::size_t size() const;
};

}  // namespace Presto
