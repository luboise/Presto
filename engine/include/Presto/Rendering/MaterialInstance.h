#pragma once

#include "Presto/Types/AssetTypes.h"
#include "Presto/Types/CoreTypes.h"

namespace Presto {

class ErasedBytes;
struct MaterialStructure;

class MaterialInstance {
    friend class MaterialAsset;

   public:
    explicit MaterialInstance(const MaterialDefinitionPtr&);

    MaterialInstance& setName(Presto::string newName);

    MaterialInstance& setProperty(Presto::string name, Presto::vec3 data);
    MaterialInstance& setProperty(Presto::string name, ErasedBytes data);
    MaterialInstance& setProperty(Presto::string name, TexturePtr data);

    [[nodiscard]] const MaterialStructure& getStructure() const;

   private:
    MaterialPtr definition_;
    Presto::string name_;
};

}  // namespace Presto
