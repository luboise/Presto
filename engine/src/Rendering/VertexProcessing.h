#include <algorithm>
#include "Presto/Assets/ImportTypes.h"

namespace Presto {

template <typename T>
    requires requires { T::vertexPosition; }
[[nodiscard]] std::vector<T> processVertices(
    const ImportedAttributeList& /*unused*/) {
    static_assert(false, "No implementation for vertex type.");
}

template <>
[[nodiscard]] std::vector<Vertex3D> processVertices<Vertex3D>(
    const ImportedAttributeList& inputAttributes) {
    std::vector<Vertex3D> vertices;

    // Get the minimum vertex count of all attributes
    Presto::size_t vertex_count{std::ranges::min(
        inputAttributes |
        std::views::transform([](const ImportedVertexAttribute& val)
                                  -> Presto::size_t { return val.count; }))};

    PR_CORE_ASSERT(
        vertex_count != 0,
        "The number of vertices to be processed must be higher than 0.");

    vertices.resize(vertex_count);

    if (const auto& in_a{std::ranges::find_if(
            inputAttributes,
            [](const ImportedVertexAttribute& val) {
                return val.name == DefaultAttributeName::POSITION;
            })};
        in_a != inputAttributes.end()) {
        // Get the desired attribute from the input list
        std::span<const Presto::float32_t> span;
        for (Presto::size_t i{0}; i < vertex_count; i++) {
            span = std::span{reinterpret_cast<const Presto::float32_t*>(
                                 &in_a->data[i * sizeof(Presto::vec3)]),
                             3};
            vertices[i].vertexPosition =
                Presto::vec3{span[0], span[1], span[2]};
        }
    } else {
        PR_ERROR(
            "Unable to find attribute {} in imported input attributes. "
            "Leaving these values as 0 and continuing with vertex "
            "processing.",
            DefaultAttributeName::POSITION);
    }

    if (const auto& in_a{std::ranges::find_if(
            inputAttributes,
            [](const ImportedVertexAttribute& val) {
                return val.name == DefaultAttributeName::NORMAL;
            })};
        in_a != inputAttributes.end()) {
        // Get the desired attribute from the input list
        Presto::size_t in_stride{sizeof(Vertex3D::normal)};

        for (Presto::size_t i{0}; i < vertex_count; i++) {
            std::memcpy(&vertices[i].normal, &in_a->data[i * in_stride],
                        in_stride);
        }
    } else {
        PR_ERROR(
            "Unable to find attribute {} in imported input attributes. "
            "Leaving these values as 0 and continuing with vertex "
            "processing.",
            DefaultAttributeName::NORMAL);
    }

    if (const auto& in_a{std::ranges::find_if(
            inputAttributes,
            [](const ImportedVertexAttribute& val) {
                return val.name == DefaultAttributeName::COLOUR;
            })};
        in_a != inputAttributes.end()) {
        // Get the desired attribute from the input list
        Presto::size_t in_stride{sizeof(Vertex3D::colour)};

        for (Presto::size_t i{0}; i < vertex_count; i++) {
            std::memcpy(&vertices[i].colour, &in_a->data[i * in_stride],
                        in_stride);
        }
    } else {
        PR_ERROR(
            "Unable to find attribute {} in imported input attributes. "
            "Leaving these values as 0 and continuing with vertex "
            "processing.",
            DefaultAttributeName::COLOUR);
    }

    if (const auto& in_a{std::ranges::find_if(
            inputAttributes,
            [](const ImportedVertexAttribute& val) {
                return val.name == DefaultAttributeName::TEXCOORDS;
            })};
        in_a != inputAttributes.end()) {
        // Get the desired attribute from the input list
        Presto::size_t in_stride{sizeof(Vertex3D::tex_coords)};

        for (Presto::size_t i{0}; i < vertex_count; i++) {
            std::memcpy(&vertices[i].tex_coords, &in_a->data[i * in_stride],
                        in_stride);
        }
    } else {
        PR_ERROR(
            "Unable to find attribute {} in imported input attributes. "
            "Leaving these values as 0 and continuing with vertex "
            "processing.",
            DefaultAttributeName::TEXCOORDS);
    }

    return vertices;
}
}  // namespace Presto
