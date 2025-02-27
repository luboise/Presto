#pragma once

#include "Presto/Assets/AssetPath.h"

namespace Presto {

using asset_name_t = PR_STRING_ID;
enum class AssetType : std::uint8_t { MESH, MODEL, MATERIAL_DEFINITION, IMAGE };

using AssetArg = AssetPath;

class MaterialAsset;
using MaterialDefinitionPtr = Ptr<MaterialAsset>;
class ImageAsset;
using ImagePtr = Ptr<ImageAsset>;
class MeshAsset;
using MeshPtr = Ptr<MeshAsset>;
class ModelAsset;
using ModelPtr = Ptr<ModelAsset>;

}  // namespace Presto
