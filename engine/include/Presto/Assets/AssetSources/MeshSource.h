#pragma once

#include "AssetSource.h"

#include "Presto/Aliases/Handles.h"
#include "Presto/Assets/ImportTypes.h"
#include "Presto/Types/AssetTypes.h"
#include "Presto/Types/CoreTypes.h"

namespace Presto {

struct ImportedModelData;

class MeshSource final : public AssetSource {
   public:
    explicit MeshSource(AssetPath filepath);

    void load() override;
    void unload() override {}

    ModelPtr getModel(const Presto::string& name);

   private:
    void updateMesh();

    Allocated<ImportedModelData> importData_;

    struct MaterialReady {
        ImportedMaterial material_import;
    };

    std::vector<ModelPtr> models_;
    std::vector<MeshPtr> meshes_;
    std::vector<MaterialPtr> materials_;
    std::vector<TexturePtr> textures_;
};

}  // namespace Presto
