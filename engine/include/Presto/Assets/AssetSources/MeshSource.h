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

    ModelPtr getModel(const Presto::string& name);
    ModelPtr loadModel(Presto::string modelName, bool allowReload = true);
    void unloadModel(const Presto::string& modelName);

    MaterialPtr getMaterial(const Presto::string& name);
    MaterialPtr loadMaterial(Presto::string materialName,
                             bool allowReload = true);
    void unloadMaterial(const Presto::string& materialName);

    void reloadFile();

    void unload() override;

    MeshSource(const MeshSource&) = delete;
    MeshSource(MeshSource&&) = delete;
    MeshSource& operator=(const MeshSource&) = delete;
    MeshSource& operator=(MeshSource&&) = delete;

   private:
    // void updateMesh();

    // Allocated<ImportedModelData> importData_;

    struct LoadedModel {
        Presto::string name;
        std::vector<ImportedMesh> mesh_imports;
        std::vector<Ptr<Mesh>> meshes;

        ModelPtr ptr{nullptr};
    };

    LoadedModel* getLoadedModel(const Presto::string& name);
    void unloadModel(LoadedModel& loadedModel);

    struct LoadedMaterial {
        Presto::string name;
        ImportedMaterial material_import;
        MaterialPtr ptr{nullptr};
    };

    LoadedMaterial* getLoadedMaterial(const Presto::string& name);
    void unloadMaterial(LoadedMaterial& loadedMaterial);

    std::vector<LoadedModel> models_;
    std::vector<LoadedMaterial> materials_;
    std::vector<TexturePtr> textures_;
};

}  // namespace Presto
