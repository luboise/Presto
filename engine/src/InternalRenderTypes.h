#pragma once

#include "Presto/Core/Constants.h"
#include "Presto/Objects/TransformData.h"

namespace Presto {

struct CanvasDrawDetails {
    mesh_registration_id_t mesh;
    TransformData transform;
};

}  // namespace Presto
