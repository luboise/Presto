#pragma once

#include "Presto/Components/Renderable.h"
#include "Presto/Core/Constants.h"
// #include "Presto/Rendering/RenderData.h"
#include "Presto/Resources/MeshResource.h"

namespace Presto {
    struct RenderData;
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
        static Mesh* New(MeshResource&);

        inline const MeshResource& getResource() { return *resource_; }

       private:
        explicit Mesh(MeshResource& resource) : resource_(&resource) {};

        [[nodiscard]] RenderData getRenderData() const override;
        MeshResource* resource_;
    };
}  // namespace Presto
