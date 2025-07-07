export module presto.assets.model;

import std;

export import presto.rendering.texture;
export import presto.assets.mesh;

import presto.assets.material;
import presto.assets.asset;
import presto.assets.importing;

import presto.core;

export namespace Pr {

class MeshSource;

class ModelAsset final : public Asset {
   public:
    explicit ModelAsset(asset_name_t);

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::MODEL;
    };

    [[nodiscard]] Pr::size_t meshCount() const;
    [[nodiscard]] const std::vector<MeshDraw>& getDraws() { return draws_; }

    BoundingBox getBoundingBox();

    ModelAsset& addMesh(Pr::Ptr<Pr::MeshAsset>,
                        Pr::Ptr<Pr::MaterialInstance> = nullptr);

    ModelAsset& clear();

   private:
    bool load() override;

    // std::vector<MeshData>

    // std::vector<RawMeshData> meshes_;
    // std::vector<MeshData> meshes_;

    std::vector<MeshDraw> draws_;
    Ptr<MeshSource> source_;
};

class MeshSource final : public AssetSource {
   public:
    explicit MeshSource(AssetPath filepath);
    ~MeshSource() override;

    [[nodiscard]] Pr::Ptr<Pr::ModelAsset> getModel(const Pr::string& name);
    [[nodiscard]] Pr::Ptr<Pr::ModelAsset> loadModel(Pr::string modelName,
                                                    bool allowReload = true);
    void unloadModel(const Pr::string& modelName);

    Pr::Ptr<Pr::MaterialInstance> getMaterial(const Pr::string& name);
    Pr::Ptr<Pr::MaterialInstance> loadMaterial(Pr::string materialName,
                                               bool allowReload = true);
    void unloadMaterial(const Pr::string& materialName);

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
        Pr::string name;
        std::vector<ImportedMesh> mesh_imports;
        std::vector<Ptr<Mesh>> meshes;

        Pr::Ptr<Pr::ModelAsset> ptr{nullptr};
    };

    LoadedModel* getLoadedModel(const Pr::string& name);
    void unloadModel(LoadedModel& loadedModel);

    struct LoadedMaterial {
        Pr::string name;
        ImportedMaterial material_import;
        Pr::Ptr<Pr::MaterialInstance> ptr{nullptr};
    };

    LoadedMaterial* getLoadedMaterial(const Pr::string& name);
    void unloadMaterial(LoadedMaterial& loadedMaterial);

    std::vector<LoadedModel> models_;
    std::vector<LoadedMaterial> materials_;
    std::vector<Pr::Ptr<Pr::Texture>> textures_;
};

}  // namespace Pr
