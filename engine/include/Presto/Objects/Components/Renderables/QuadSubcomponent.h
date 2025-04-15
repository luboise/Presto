#pragma once

#include "Presto/Aliases/Handles.h"

#include "Presto/Objects/Components/RenderComponent.h"
#include "Presto/Objects/Subcomponent.h"
#include "Presto/Objects/TransformData.h"

namespace Presto {

struct PRESTO_API QuadSubcomponent : public Subcomponent<RenderComponent> {
    float width;
    float height;

    TransformData transform{};
    MaterialPtr material;
};

}  // namespace Presto
