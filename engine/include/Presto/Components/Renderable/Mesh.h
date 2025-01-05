#pragma once

#include "Presto/Components/Renderable.h"
#include "Presto/Resources/MeshResource.h"
#include "PrestoCore/Core/Constants.h"
#include "PrestoCore/Rendering/RenderData.h"

namespace Presto {
    // #define PrestoMeshConstructorArgs mesh_id_t, const VertexList&, const
    // IndexList&

    struct MeshCreationInfo {
        VertexList vertices;
        IndexList indices;
    };

    using mesh_id_t = PR_NUMERIC_ID;

    class PRESTO_API Mesh : public Renderable {
        // friend class RenderingManager;

       public:
        explicit Mesh(MeshResource& resource) : resource_(&resource) {};

        inline const MeshResource& getResource() { return *resource_; }

       private:
        [[nodiscard]] RenderData getRenderData() const override;
        MeshResource* resource_;
    };
}  // namespace Presto
