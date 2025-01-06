#include "Presto/Components/Renderable/Mesh.h"

#include "Presto/Modules/EntityManager.h"
#include "Presto/Rendering/RenderData.h"

namespace Presto {
    RenderData Mesh::getRenderData() const {
        // TODO: Implement this
        return {};
    };

    Mesh* Mesh::New(MeshResource& resource) {
        auto& em{EntityManager::Get()};

        Mesh* m{em.newComponent<Mesh>(resource)};

        return m;
    }

}  // namespace Presto
