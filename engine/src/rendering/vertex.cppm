export module presto.internal.rendering:vertex;

import presto.core.types;

#include "AttributeTypes.h"

#include "Presto/Rendering/ShaderTypes.h"

#include <map>
#include <variant>

export namespace Presto {

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

using vertex_binding_t = std::uint16_t;

struct VertexAttribute {
    // AttributeTypeDetails typeDetails;
    ShaderDataType type;

    Presto::string name;
    vertex_binding_t index;
    Presto::size_t offset{0};

    [[nodiscard]] std::size_t size() const noexcept {
        return SizeOfShaderType(type);
    };
};

class AttributeSet {
   public:
    void addAttribute(VertexAttribute attrib);

    [[nodiscard]] const VertexAttribute* getAttribute(
        const Presto::string& name) const;

    [[nodiscard]] const auto& getAttributes() const;

    [[nodiscard]] std::size_t size();

    bool lock(bool calculateOffsets = false);

    [[nodiscard]] bool locked() const { return locked_; }

    [[nodiscard]] std::size_t bytesRequired() const;

    // Alias of bytesRequired
    [[nodiscard]] std::size_t stride() const { return bytesRequired(); }

   private:
    bool locked_{false};

    std::map<vertex_binding_t, VertexAttribute> attributes_;
};
}  // namespace Presto
