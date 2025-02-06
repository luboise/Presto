#pragma once

#include "Presto/Assets/ImportTypes.h"
#include "Presto/Core/Types.h"

namespace Presto {

class ModelLoader {
   public:
    virtual ~ModelLoader() = default;

    virtual ImportedModelData load(
        const AssetPath& filepath,
        const std::vector<asset_name_t>& customNames) = 0;
};

}  // namespace Presto
