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
    ~MeshSource() override;

    ModelPtr loadModel(Presto::string modelName, bool allowReload = true);
    void unloadModel(const Presto::string& modelName);

    void reloadFile();

    ModelPtr getModel(const Presto::string& name);

   private:
    // void updateMesh();

    // Allocated<ImportedModelData> importData_;

    struct LoadedModel {
        Presto::string name;
        std::vector<ImportedMesh> mesh_imports;
        std::vector<Ptr<Mesh>> meshes;

        ModelPtr ptr;
    };

    LoadedModel* getLoadedModel(const Presto::string& name);
    void unloadModel(LoadedModel& loadedModel);

    struct LoadedMaterial {
        ImportedMaterial material_import;
        MaterialPtr ptr;
    };

    std::vector<LoadedModel> models_;
    std::vector<LoadedMaterial> materials_;
    std::vector<TexturePtr> textures_;
};

}  // namespace Presto
