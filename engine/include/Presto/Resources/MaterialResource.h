#pragma once

#include "Presto/Core/Types.h"
#include "Presto/Resources/Image.h"

namespace Presto {
    struct MaterialResource {
        friend class ResourceManager;

        PR_STRING_ID name;

        Presto::Image image;
    };
}  // namespace Presto
