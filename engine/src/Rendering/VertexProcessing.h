#include <algorithm>
#include "Presto/Assets/ImportTypes.h"
#include "Presto/Rendering/PipelineTypes.h"

namespace Presto {

[[nodiscard]] ProcessedMeshVertices processVertices3D(
    const ImportedAttributeList& inputAttributes) {
    ProcessedMeshVertices processed_data{};

    // Get the minimum vertex count of all attributes
    Presto::size_t vertex_count{std::ranges::min(
        inputAttributes |
        std::views::transform([](const ImportedVertexAttribute& val)
                                  -> Presto::size_t { return val.count; }))};

    processed_data.vertices.resize(vertex_count);

    if (const auto& in_a{std::ranges::find_if(
            inputAttributes,
            [](const ImportedVertexAttribute& val) {
                return val.name == DefaultAttributeName::POSITION;
            })};
        in_a != inputAttributes.end()) {
        // Get the desired attribute from the input list
        Presto::size_t in_stride{sizeof(Vertex3D::vertexPosition)};

        for (Presto::size_t i{0}; i < vertex_count; i++) {
            std::memcpy(&processed_data.vertices[i].vertexPosition,
                        &in_a->data[i * in_stride], in_stride);
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
            std::memcpy(&processed_data.vertices[i].normal,
                        &in_a->data[i * in_stride], in_stride);
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
            std::memcpy(&processed_data.vertices[i].colour,
                        &in_a->data[i * in_stride], in_stride);
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
            std::memcpy(&processed_data.vertices[i].tex_coords,
                        &in_a->data[i * in_stride], in_stride);
        }
    } else {
        PR_ERROR(
            "Unable to find attribute {} in imported input attributes. "
            "Leaving these values as 0 and continuing with vertex "
            "processing.",
            DefaultAttributeName::TEXCOORDS);
    }

    processed_data.vertex_count = vertex_count;

    return processed_data;
}

}  // namespace Presto
