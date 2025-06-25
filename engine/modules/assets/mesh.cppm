export module presto.assets.mesh;

import presto.assets.types;
import presto.core;

export namespace Pr {

// TODO: Move these
class MaterialInstance;
using MaterialPtr = Ptr<MaterialInstance>;

class Texture;
using TexturePtr = Ptr<Texture>;

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
    [[nodiscard]] BoundingBox getBoundingBox() const;  // namespace Pr

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

    [[nodiscard]] ModelPtr getModel(const Pr::string& name);
    [[nodiscard]] ModelPtr loadModel(Pr::string modelName,
                                     bool allowReload = true);
    void unloadModel(const Pr::string& modelName);

    MaterialPtr getMaterial(const Pr::string& name);
    MaterialPtr loadMaterial(Pr::string materialName, bool allowReload = true);
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

        ModelPtr ptr{nullptr};
    };

    LoadedModel* getLoadedModel(const Pr::string& name);
    void unloadModel(LoadedModel& loadedModel);

    struct LoadedMaterial {
        Pr::string name;
        ImportedMaterial material_import;
        MaterialPtr ptr{nullptr};
    };

    LoadedMaterial* getLoadedMaterial(const Pr::string& name);
    void unloadMaterial(LoadedMaterial& loadedMaterial);

    std::vector<LoadedModel> models_;
    std::vector<LoadedMaterial> materials_;
    std::vector<TexturePtr> textures_;
};

struct BoundingBox {
    double x_min{};
    double x_max{};

    double y_min{};
    double y_max{};

    double z_min{};
    double z_max{};

    void merge(const BoundingBox& other);

    /**
     *  @brief Calculates the co-efficient necessary in order to clamp a
     * bounding box to a 1x1x1 cube.
     * @warning At least one value of the bounding box must be
     * non-zero, otherwise 0 will be returned. This is likely to cause a
     * divide by zero somewhere.
     *  @return   The normalisation co-efficient.
     *
     * For example, if the bounding box has an x_min of -0.5, an x_max
     * of 1.5, a y_min of 0, a y_max of 2.0, a z_min of 0.1 and a z_max of
     * 0.3, the model will be scaled so that all values are between [-0.5,
     * 0.5].
     */
    [[nodiscard]] double getNormalisingFactor() const;
};

}  // namespace Pr
