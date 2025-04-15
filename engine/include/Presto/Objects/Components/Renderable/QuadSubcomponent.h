#pragma once

#include "Presto/Aliases/Handles.h"

#include "Presto/Objects/Components/RenderComponent.h"
#include "Presto/Objects/Subcomponent.h"

namespace Presto {

struct PRESTO_API QuadSubcomponent : public Subcomponent<RenderComponent> {
    TexturePtr texture;
};

}  // namespace Presto
