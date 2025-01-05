#include <utility>

#include "Presto/Components/Renderable/Mesh.h"

namespace Presto {
    Mesh::Mesh(VertexList vertices, IndexList indices)
        : vertices_(std::move(vertices)), indices_(std::move(indices)) {

          };

    [[nodiscard]] RenderData Mesh::getRenderData() const {
        RenderData render_data{vertices_, indices_};

        return render_data;
    };

}  // namespace Presto
