#include "Presto/Assets/ImportTypes.h"
#include "Presto/Rendering/PipelineTypes.h"

namespace Presto {

[[nodiscard]] ProcessedVertexData processVertices(
    const ImportedAttributeList& importedAttributes,
    const std::vector<PipelineAttribute>& pipelineAttributes) {
    ProcessedVertexData processed_data{};
    /*
MeshData mesh_data;

mesh_data.indices.resize(rawData.indices.size());

std::ranges::transform(rawData.indices.begin(), rawData.indices.end(),
                       mesh_data.indices.begin(),
                       [](auto val) { return static_cast<Index>(val); });

mesh_data.vertices.resize(
    std::min({rawData.positions.size(), rawData.normals.size(),
              rawData.tex_coords.size()}));

for (std::size_t i = 0; i < mesh_data.vertices.size(); i++) {
    Vertex v = {.position = rawData.positions[i],
                // Default colour of white
                .colour = {1, 1, 1},
                .normal = rawData.normals[i],
                .tex_coords = rawData.tex_coords[i]};
    mesh_data.vertices[i] = v;
}

mesh_data.draw_mode = rawData.draw_mode;
// data.material = rawData.defaultMaterial_;
    */

    static_assert(false, "Implement process vertices");
    return processed_data;
}

}  // namespace Presto
