#pragma once

#include "Presto/Assets/ImportTypes.h"

#include "Presto/Assets/MaterialAsset.h"

#include "Presto/Assets/Asset.h"

#include "Presto/Assets/ImageAsset.h"

namespace Presto {

class MaterialAsset;
class MaterialInstanceProperty;

class MaterialInstanceAsset {
   public:

   private:
    std::vector<ImagePtr> images_;
};
}  // namespace Presto
