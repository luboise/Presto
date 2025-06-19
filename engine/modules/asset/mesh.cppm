export module presto.assets.mesh;

#include "Presto/Aliases/Handles.h"
#include "Presto/Assets/Asset.h"

#include "Presto/Core/Constants.h"

#include "Presto/Rendering/BoundingBox.h"

#include "ImportTypes.h"

export namespace Presto {

class Mesh {
    friend class RenderingManager;

   public:
    [[nodiscard]] mesh_registration_id_t registrationId() const;

   private:
    explicit Mesh(mesh_registration_id_t id);
    mesh_registration_id_t registrationId_{PR_UNREGISTERED};
};

struct MeshData;

class MeshAsset final : public Asset {
    friend class RenderingManager;

   public:
    explicit MeshAsset();
    ~MeshAsset() override;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MESH;
    };

    [[nodiscard]] mesh_registration_id_t registrationId() const;

    /*
MeshAsset& setVertices(const ImportedAttributeList& attributes);
MeshAsset& setIndices(IndexList indices);

MeshAsset& setDrawMode(MeshDrawMode mode);
    */

    MeshAsset& setMeshData(MeshData data);

    [[nodiscard]] MaterialPtr& defaultMaterial() const;
    MeshAsset& setDefaultMaterial(const MaterialPtr&);

    // TODO: Adapt to imported mesh so it doesn't have to be calculated
    [[nodiscard]] BoundingBox getBoundingBox() const;  // namespace Presto

    static MeshPtr from(const ImportedMesh&);

   private:
    struct Impl;

    Allocated<Impl> impl_;

    bool load() override;

    [[nodiscard]] bool modifiable() const;
};

class MeshSource final : public AssetSource {
   public:
    explicit MeshSource(AssetPath filepath);
    ~MeshSource() override;

    [[nodiscard]] ModelPtr getModel(const Presto::string& name);
    [[nodiscard]] ModelPtr loadModel(Presto::string modelName,
                                     bool allowReload = true);
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
