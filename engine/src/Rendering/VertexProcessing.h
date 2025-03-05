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

    vertices.resize(vertex_count);

    if (const auto& in_a{std::ranges::find_if(
            inputAttributes,
            [](const ImportedVertexAttribute& val) {
                return val.name == DefaultAttributeName::POSITION;
            })};
        in_a != inputAttributes.end()) {
        // Get the desired attribute from the input list
        Presto::size_t in_stride{sizeof(Vertex3D::vertexPosition)};

        for (Presto::size_t i{0}; i < vertex_count; i++) {
            std::memcpy(&vertices[i].vertexPosition, &in_a->data[i * in_stride],
                        in_stride);
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
