#include <map>
#include <variant>
#include <vector>

import :base;

#include "presto/core/types.h"

namespace Presto {

struct PipelineAttribute;

/*
struct Vertex {
    [[nodiscard]] constexpr std::vector<PipelineAttribute>
    getPipelineAttributes();

    Vertex() = default;
    ~Vertex() = default;

    Vertex(const Vertex&) = default;
    Vertex& operator=(const Vertex&) = default;
    Vertex(Vertex&&) = default;
    Vertex& operator=(Vertex&&) = default;
};
*/
struct Vertex {};

struct Vertex3D : public Vertex {
    Presto::vec3 vertexPosition;
    Presto::vec3 colour = {1, 1, 1};
    Presto::vec3 normal;
    Presto::vec2 tex_coords;

    [[nodiscard]] static std::vector<PipelineAttribute> getPipelineAttributes();
};

struct VertexUI : public Vertex {
    Presto::vec2 vertexPosition;
    Presto::vec4 colour;
    Presto::vec2 tex_coords;

    [[nodiscard]] static std::vector<PipelineAttribute> getPipelineAttributes();
};

#ifndef NDEBUG
struct VertexDebug : public Vertex {
    Presto::vec3 vertexPosition;
    Presto::vec4 colour{1, 1, 1, 1};

    [[nodiscard]] static std::vector<PipelineAttribute> getPipelineAttributes();
};
#endif

using AnyVertexType = std::variant<Vertex3D, VertexUI>;
using AnyVertexList =
    std::variant<std::vector<Vertex3D>, std::vector<VertexUI>>;

using vertex_binding_t = Presto::uint16_t;

struct VertexAttribute {
    // AttributeTypeDetails typeDetails;
    ShaderDataType type;

    Presto::string name;
    vertex_binding_t index;
    Presto::size_t offset{0};

    [[nodiscard]] Presto::size_t size() const noexcept {
        return SizeOfShaderType(type);
    };
};

class AttributeSet {
   public:
    void addAttribute(VertexAttribute attrib);

    [[nodiscard]] const VertexAttribute* getAttribute(
        const Presto::string& name) const;

    [[nodiscard]] const auto& getAttributes() const;

    [[nodiscard]] Presto::size_t size();

    bool lock(bool calculateOffsets = false);

    [[nodiscard]] bool locked() const { return locked_; }

    [[nodiscard]] Presto::size_t bytesRequired() const;

    // Alias of bytesRequired
    [[nodiscard]] Presto::size_t stride() const { return bytesRequired(); }

   private:
    bool locked_{false};

    std::map<vertex_binding_t, VertexAttribute> attributes_;
};

using attribute_size_t = Presto::size_t;

struct BaseAttributeTypeDetails {
    Presto::size_t subtype_size;
    Presto::size_t count;
    Presto::size_t size;
};

template <typename T>
// TODO: Fix this constraint
// requires requires { SubTypeDetails<T>::subtype; }
struct AttributeTypeDetails : BaseAttributeTypeDetails {
    using details = SubTypeDetails<T>;
    using subtype = details::subtype;

    static constexpr Presto::size_t subtype_size = sizeof(subtype);
    static constexpr Presto::size_t count{details::subtype_count};
    static constexpr Presto::size_t size = subtype_size * count;
};

}  // namespace Presto
