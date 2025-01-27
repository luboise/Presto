#pragma once

#include "Presto/Components/Renderable.h"
#include "Presto/Core/Constants.h"
#include "Presto/Resources/MaterialResource.h"
#include "Presto/Resources/MeshResource.h"

namespace Presto {
    struct RenderGroup;

    using mesh_id_t = PR_NUMERIC_ID;

    class PRESTO_API MeshGroup : public Renderable {
        // friend class RenderingManager;
        friend class EntityManager;

        // TODO: Make the render trigger when the entity enters the scene so
        // that this friend declaration isn't necessary
        friend class RenderingManager;

       public:
        [[nodiscard]] bool hasResource() const;

        inline MeshResource* getMesh(std::uint8_t index) {
            return meshResources_[index];
        }
        inline void setMesh(std::uint8_t index, MeshResource* mesh) {
            meshResources_[index] = mesh;
        }

        inline MaterialResource* getMaterial(std::size_t index) {
            return materialResources_[index];
        };
        void setMaterial(std::uint8_t index, MaterialResource* resource) {
            materialResources_[index] = resource;
        };

       private:
        MeshResource* getMeshResource() { return meshResource_; }

        explicit MeshGroup(MeshResource& resource);
        MeshGroup() = default;

        std::vector<MeshResource*> meshResources_;
        std::vector<MaterialResource*> materialResources_;

        // MeshResource* meshResource_{nullptr};
        // MaterialResource* materialResource_{nullptr};
    };
}  // namespace Presto
