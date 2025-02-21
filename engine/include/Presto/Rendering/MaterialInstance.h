#pragma once

#include "Presto/Core/Types.h"

namespace Presto {

class MaterialAsset;

class MaterialInstance {
    friend class MaterialAsset;

   public:
    explicit MaterialInstance(const Ptr<MaterialAsset>&);

    MaterialInstance& setProperty(Presto::string name, Presto::vec3 data);

   private:
    Ref<MaterialAsset> definition_;
};

using MaterialPtr = Ref<MaterialInstance>;

}  // namespace Presto
