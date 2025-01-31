#pragma once

#include "Presto/Core/Constants.h"

#include "Presto/Assets/ImageAsset.h"
#include "Presto/Objects/Component.h"

namespace Presto {
    struct RenderGroup;

    using mesh_id_t = PR_NUMERIC_ID;

    class PRESTO_API SpriteComponent : public Component {
        // friend class RenderingManager;
        friend class EntityManager;

       public:
        [[nodiscard]] bool hasAsset() const;

        ImagePtr getAsset();

        void setAsset(const ImagePtr& asset);

       private:
        explicit SpriteComponent(const ImagePtr& asset);
        SpriteComponent() = default;

        ImagePtr asset_{nullptr};
    };
}  // namespace Presto
