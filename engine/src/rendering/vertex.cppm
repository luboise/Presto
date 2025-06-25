module;
#include <map>
#include <variant>
#include <vector>

export module presto.internal.rendering:vertex;
import :base;

import presto.core.types;

export namespace Pr {

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
    Pr::vec3 vertexPosition;
    Pr::vec3 colour = {1, 1, 1};
    Pr::vec3 normal;
    Pr::vec2 tex_coords;

    [[nodiscard]] static std::vector<PipelineAttribute> getPipelineAttributes();
};

struct VertexUI : public Vertex {
    Pr::vec2 vertexPosition;
    Pr::vec4 colour;
    Pr::vec2 tex_coords;

    [[nodiscard]] static std::vector<PipelineAttribute> getPipelineAttributes();
};

#ifndef NDEBUG
struct VertexDebug : public Vertex {
    Pr::vec3 vertexPosition;
    Pr::vec4 colour{1, 1, 1, 1};

    [[nodiscard]] static std::vector<PipelineAttribute> getPipelineAttributes();
};
#endif

using AnyVertexType = std::variant<Vertex3D, VertexUI>;
using AnyVertexList =
    std::variant<std::vector<Vertex3D>, std::vector<VertexUI>>;

using vertex_binding_t = Pr::uint16_t;

struct VertexAttribute {
    // AttributeTypeDetails typeDetails;
    ShaderDataType type;

    Pr::string name;
    vertex_binding_t index;
    Pr::size_t offset{0};

    [[nodiscard]] Pr::size_t size() const noexcept {
        return SizeOfShaderType(type);
    };
};

class AttributeSet {
   public:
    void addAttribute(VertexAttribute attrib);

    [[nodiscard]] const VertexAttribute* getAttribute(
        const Pr::string& name) const;

    [[nodiscard]] const auto& getAttributes() const;

    [[nodiscard]] Pr::size_t size();

    bool lock(bool calculateOffsets = false);

    [[nodiscard]] bool locked() const { return locked_; }

    [[nodiscard]] Pr::size_t bytesRequired() const;

    // Alias of bytesRequired
    [[nodiscard]] Pr::size_t stride() const { return bytesRequired(); }

   private:
    bool locked_{false};

    std::map<vertex_binding_t, VertexAttribute> attributes_;
};

using attribute_size_t = Pr::size_t;

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

}  // namespace Pr
