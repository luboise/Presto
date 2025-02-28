#include <algorithm>
#include "Presto/Assets/ImportTypes.h"
#include "Presto/Rendering/PipelineTypes.h"

namespace Presto {

[[nodiscard]] ProcessedVertexData processVertices(
    const ImportedAttributeList& inputAttributes,
    const PipelineStructure& pipelineStructure) {
    ProcessedVertexData processed_data{};

    // Get the minimum vertex count of all attributes
    Presto::size_t vertex_count{std::ranges::min(
        inputAttributes |
        std::views::transform([](const ImportedVertexAttribute& val)
                                  -> Presto::size_t { return val.count; }))};

    Presto::size_t out_stride{pipelineStructure.stride()};

    const auto out_size{out_stride * vertex_count};
    processed_data.data.resize(out_size);

    // Get the desired attribute from the input list
    for (const PipelineAttribute& out_a : pipelineStructure.attributes) {
        const auto& in_a{std::ranges::find_if(
            inputAttributes, [out_a](const ImportedVertexAttribute& val) {
                return val.name == out_a.name;
            })};
        if (in_a == inputAttributes.end()) {
            PR_ERROR(
                "Unable to find attribute {} in imported input attributes. "
                "Leaving these values as 0 and continuing with vertex "
                "processing.",
                out_a.name);
            continue;
        }

        Presto::size_t out_stride{in_a->dataSize()};
        Presto::size_t in_stride{in_a->dataSize()};

        Presto::size_t out_offset{out_a.offset};

        for (Presto::size_t i{0}; i < vertex_count; i++) {
            std::memcpy(&processed_data.data[out_offset + (i * out_stride)],
                        &in_a->data[i * in_stride], in_stride);
        }
    }

    processed_data.vertex_count = vertex_count;

    return processed_data;
}

}  // namespace Presto
