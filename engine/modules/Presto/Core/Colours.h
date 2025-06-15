#pragma once

#include "Presto/Types/CoreTypes.h"

namespace Presto {

using colour_t = Presto::vec4;

constexpr struct {
    colour_t WHITE{1, 1, 1, 1};
    colour_t BLACK{0, 0, 0, 1};

    colour_t RED{1, 0, 0, 1};
    colour_t GREEN{0, 1, 0, 1};
    colour_t BLUE{0, 0, 1, 1};

    colour_t PURPLE{1, 0, 1, 1};
} Colour;

}  // namespace Presto
