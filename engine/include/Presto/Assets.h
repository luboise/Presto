#pragma once

/*
#include "Presto/Assets/ImageAsset.h"     // IWYU pragma: export
#include "Presto/Assets/MaterialAsset.h"  // IWYU pragma: export
#include "Presto/Assets/MeshAsset.h"      // IWYU pragma: export
#include "Presto/Assets/ModelAsset.h"     // IWYU pragma: export
*/
#include "Presto/Types/AssetTypes.h"

#include "Presto/Assets/ModelAsset.h"  // IWYU pragma: export

namespace Presto {

ImagePtr LoadImage(const AssetArg& path);

/**
 * @brief Loads models from the disk, and returns a list of loaded models.
 */
ModelPtr LoadModel(const AssetArg& filepath,
                   const asset_name_t& customName = "");

/**
 * @brief  Finds an existing (loaded) model, and returns a new handle to it.
 */
ModelPtr FindModel(const asset_name_t& name);

}  // namespace Presto
