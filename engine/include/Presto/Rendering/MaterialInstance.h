#pragma once

namespace Presto {

class MaterialAsset;

class MaterialInstance {
    friend class MaterialAsset;

   public:
    explicit MaterialInstance(const Ptr<MaterialAsset>&);

   private:
    Ref<MaterialAsset> definition_;
};

using MaterialPtr = Ref<MaterialInstance>;

}  // namespace Presto
