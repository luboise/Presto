#include "Presto/Components/Renderable/Mesh.h"

namespace Presto {
    [[nodiscard]] RenderData Mesh::getRenderData() const {
        RenderData render_data{_vertices, _indices};

        return render_data;
    };

}  // namespace Presto
