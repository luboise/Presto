#pragma once

#include "Presto/Core/Constants.h"

#include "Presto/Objects/Component.h"
#include "Presto/Objects/Components/RenderComponent.h"
#include "Presto/Objects/Subcomponent.h"

namespace Presto {

using mesh_registration_id_t = PR_NUMERIC_ID;

struct PRESTO_API SpriteSubcomponent : public Subcomponent<RenderComponent> {
    // friend class RenderingManager;
    friend class EntityManagerImpl;

   public:
    [[nodiscard]] bool hasAsset() const;

    ImagePtr getAsset();

    void setAsset(const ImagePtr& asset);

   private:
    explicit SpriteSubcomponent(const ImagePtr& asset);
    SpriteSubcomponent() = default;

    ImagePtr asset_{nullptr};
};
}  // namespace Presto
