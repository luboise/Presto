export module presto.assets.mesh;

import std;
import presto.core;

import presto.types;

import presto.assets.asset;
import presto.assets.material;

export namespace Pr {

class Mesh {
   public:
    // TODO: Make this hidden
    explicit Mesh(mesh_registration_id_t id);
    [[nodiscard]] mesh_registration_id_t registrationId() const;

   private:
    mesh_registration_id_t registrationId_{PR_UNREGISTERED};
};

struct BoundingBox;

class MeshAsset final : public Asset {
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

    [[nodiscard]] Pr::Ptr<Pr::MaterialInstance>& defaultMaterial() const;
    MeshAsset& setDefaultMaterial(const Pr::Ptr<Pr::MaterialInstance>&);

    // TODO: Adapt to imported mesh so it doesn't have to be calculated
    [[nodiscard]] BoundingBox getBoundingBox() const;  // namespace Pr

    // static Pr::Ptr<Pr::MeshAsset> from(const ImportedMesh&);

   private:
    struct Impl;

    Allocated<Impl> impl_;

    bool load() override;

    [[nodiscard]] bool modifiable() const;
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

struct MeshDraw {
    const Pr::Ptr<Pr::MeshAsset> mesh;
    const Pr::Ptr<Pr::MaterialInstance> material;
};

}  // namespace Pr
