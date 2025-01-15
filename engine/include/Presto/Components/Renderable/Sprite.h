#pragma once

#include "Presto/Components/Renderable.h"
#include "Presto/Core/Constants.h"
// #include "Presto/Rendering/RenderData.h"
#include "Presto/Resources/ImageResource.h"

namespace Presto {
    struct RenderData;
    // #define PrestoSpriteConstructorArgs mesh_id_t, const VertexList&, const
    // IndexList&

    struct SpriteCreationInfo {
        VertexList vertices;
        IndexList indices;
    };

    using mesh_id_t = PR_NUMERIC_ID;

    class PRESTO_API Sprite : public Renderable {
        // friend class RenderingManager;
        friend class EntityManager;

       public:
        [[nodiscard]] bool hasResource() const;

        inline const ImageResource& getResource() { return *resource_; }

        void setResource(ImageResource& resource);

       private:
        explicit Sprite(ImageResource& resource);
        Sprite() = default;

        [[nodiscard]] RenderData getRenderData() const override;
        ImageResource* resource_{nullptr};
    };
}  // namespace Presto
