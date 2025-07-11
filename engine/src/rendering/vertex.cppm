export module presto.internal.rendering:vertex;

import std;

import :renderer;

import presto.types.core;
import presto.assets.importing;

import presto.rendering.vertices;

export namespace Pr {

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

#ifndef NDEBUG
struct VertexDebug : public Vertex {
    Pr::vec3 vertexPosition;
    Pr::vec4 colour{1, 1, 1, 1};
};
#endif

template <typename T>
struct VertexTypeTraits {
    static_assert(false, "No type trait instantiation defined.");
};

template <>
struct VertexTypeTraits<Vertex3D> {
    static constexpr auto PipelineAttributes =
        std::to_array({PipelineAttribute{.layout = 0,
                                         .type = ShaderDataType::VEC3,
                                         .name = DefaultAttributeName::POSITION,
                                         .offset = 0},
                       PipelineAttribute{.layout = 1,
                                         .type = ShaderDataType::VEC3,
                                         .name = DefaultAttributeName::COLOUR,
                                         .offset = 12},
                       PipelineAttribute{.layout = 2,
                                         .type = ShaderDataType::VEC3,
                                         .name = DefaultAttributeName::NORMAL,
                                         .offset = 24},
                       PipelineAttribute{
                           .layout = 3,
                           .type = ShaderDataType::VEC2,
                           .name = DefaultAttributeName::TEXCOORDS,
                           .offset = 36,
                       }});
};  // namespace Pr

template <>
struct VertexTypeTraits<VertexUI> {
    static constexpr auto PipelineAttributes = std::to_array({
        PipelineAttribute{.layout = 0,
                          .type = ShaderDataType::VEC2,
                          .name = DefaultAttributeName::POSITION,
                          .offset = 0},
        PipelineAttribute{.layout = 1,
                          .type = ShaderDataType::VEC4,
                          .name = DefaultAttributeName::COLOUR,
                          .offset = 8},
        PipelineAttribute{.layout = 2,
                          .type = ShaderDataType::VEC2,
                          .name = DefaultAttributeName::TEXCOORDS,
                          .offset = 20},
    });
};

template <>
struct VertexTypeTraits<VertexDebug> {
    static constexpr auto PipelineAttributes = std::to_array({
        PipelineAttribute{.layout = 0,
                          .type = ShaderDataType::VEC3,
                          .name = DefaultAttributeName::POSITION,
                          .offset = 0},
        PipelineAttribute{.layout = 1,
                          .type = ShaderDataType::VEC4,
                          .name = DefaultAttributeName::COLOUR,
                          .offset = 12},
    });
};

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

}  // namespace Pr
