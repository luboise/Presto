#pragma once

#include "presto/handles/core_handles.h"

namespace Pr {

class ImageAsset;
using ImagePtr = Ptr<ImageAsset>;

class ModelAsset;
using ModelPtr = Ptr<ModelAsset>;

class Texture;
using TexturePtr = Ptr<Texture>;

class FontAsset;
using FontPtr = Ptr<FontAsset>;

class MeshAsset;
using MeshPtr = Ptr<MeshAsset>;

class MaterialAsset;
using MaterialDefinitionPtr = Ptr<MaterialAsset>;

class MaterialInstance;
using MaterialPtr = Ptr<MaterialInstance>;

}  // namespace Pr
