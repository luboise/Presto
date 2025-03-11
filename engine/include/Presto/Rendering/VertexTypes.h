#pragma once

#include <variant>

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
    Presto::vec3 colour;
    Presto::vec3 normal;
    Presto::vec2 tex_coords;

    [[nodiscard]] static std::vector<PipelineAttribute> getPipelineAttributes();
};

struct VertexUI : public Vertex {
    Presto::vec2 vertexPosition;
    Presto::vec3 colour;
    Presto::vec2 tex_coords;

    [[nodiscard]] static std::vector<PipelineAttribute> getPipelineAttributes();
};

using AnyVertexType = std::variant<Vertex3D, VertexUI>;
using AnyVertexList =
    std::variant<std::vector<Vertex3D>, std::vector<VertexUI>>;

}  // namespace Presto
