#pragma once

#include "Presto/Core.h"

/*
#include "Presto/Assets/ImageAsset.h"     // IWYU pragma: export
#include "Presto/Assets/MaterialAsset.h"  // IWYU pragma: export
#include "Presto/Assets/MeshAsset.h"      // IWYU pragma: export
#include "Presto/Assets/ModelAsset.h"     // IWYU pragma: export
*/
#include "Presto/Assets/AssetSources/AssetSource.h"  // IWYU pragma: export
#include "Presto/Assets/AssetSources/MeshSource.h"   // IWYU pragma: export

#include "Presto/Types/AssetTypes.h"

#include "Presto/Assets/ModelAsset.h"  // IWYU pragma: export

namespace Presto {

class MeshSource;

/**
 * @brief Loads an image from the disk, and creates a new ImageAsset out of it.
 * If no name is given, the name of the file is used as the name of the asset.
 */
ImagePtr LoadImage(const AssetArg& path, Presto::string name = "");

/**
 * @brief Loads models from the disk, and returns a list of loaded models.
 */
ModelPtr LoadModel(const AssetArg& filepath,
                   const asset_name_t& customName = "");

/**
 * @brief  Finds an existing (loaded) model, and returns a new handle to it.
 */
ModelPtr FindModel(const asset_name_t& name);

Ptr<MeshSource> AddMeshSource(const AssetArg& filepath);

}  // namespace Presto
