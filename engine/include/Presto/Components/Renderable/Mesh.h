#pragma once

#include "Presto/Components/Renderable.h"
#include "Presto/Core/Constants.h"
#include "Presto/Resources/MeshResource.h"

namespace Presto {
    struct RenderGroup;
    // #define PrestoMeshConstructorArgs mesh_id_t, const VertexList&, const
    // IndexList&

    struct MeshCreationInfo {
        VertexList vertices;
        IndexList indices;
    };

    using mesh_id_t = PR_NUMERIC_ID;

    class PRESTO_API Mesh : public Renderable {
        // friend class RenderingManager;
        friend class EntityManager;

       public:
        [[nodiscard]] bool hasResource() const;

        inline const MeshResource& getResource() { return *resource_; }

        void setResource(MeshResource& resource);

       private:
        explicit Mesh(MeshResource& resource);
        Mesh() = default;

        MeshResource* resource_{nullptr};
    };
}  // namespace Presto
