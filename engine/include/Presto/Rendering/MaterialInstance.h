#pragma once

#include "Presto/Core/Types.h"

#include "Presto/Rendering/ErasedBytes.h"

namespace Presto {

class MaterialAsset;

class MaterialInstance {
    friend class MaterialAsset;

   public:
    explicit MaterialInstance(const Ptr<MaterialAsset>&);

    MaterialInstance& setProperty(Presto::string name, Presto::vec3 data);
    MaterialInstance& setProperty(Presto::string name, ErasedBytes data);

   private:
    Ref<MaterialAsset> definition_;
};

using MaterialPtr = Ptr<MaterialInstance>;

}  // namespace Presto
