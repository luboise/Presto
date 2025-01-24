#pragma once

#include "Presto/Components/Renderable.h"
#include "Presto/Core/Constants.h"
#include "Presto/Resources/MaterialResource.h"
#include "Presto/Resources/MeshResource.h"

namespace Presto {
    struct RenderGroup;

    using mesh_id_t = PR_NUMERIC_ID;

    class PRESTO_API Mesh : public Renderable {
        // friend class RenderingManager;
        friend class EntityManager;

       public:
        [[nodiscard]] bool hasResource() const;

        inline const MeshResource& getMesh() { return *meshResource_; }
        void setMesh(MeshResource& resource);

        inline const MaterialResource& getMaterial() {
            return *materialResource_;
        };
        void setMaterial(MaterialResource& resource);

       private:
        explicit Mesh(MeshResource& resource);
        Mesh() = default;

        MeshResource* meshResource_{nullptr};
        MaterialResource* materialResource_{nullptr};
    };
}  // namespace Presto
