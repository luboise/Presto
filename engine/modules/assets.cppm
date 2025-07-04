export module presto.assets;

export import presto.assets.font;
export import presto.assets.image;
export import presto.assets.material;
export import presto.assets.model;

export namespace Pr {

// Materials
Pr::Ptr<Pr::MaterialInstance> NewMaterial(MaterialType type,
                                          Pr::string name = "");
Pr::Ptr<Pr::MaterialInstance> FindMaterial(const Pr::string& name);

// Textures
/**
 * @brief  Creates a new texture based on a filepath. This is the equivalent to
 * loading the image first to obtain an Pr::Ptr<Pr::ImageAsset>, then calling
 * loadTexture2D(ptr)
 */
Ptr<Texture2D> NewTexture2D(AssetArg path);
Ptr<Texture2D> NewTexture2D(const Pr::Ptr<Pr::ImageAsset>& ptr);

/**
 * @brief Loads an image from the disk, and creates a new ImageAsset out of it.
 * If no name is given, the name of the file is used as the name of the asset.
 */
Pr::Ptr<Pr::ImageAsset> LoadImage(const AssetArg& path, Pr::string name = "");

/**
 * @brief  Creates a new MeshSource object from a file path. This can be used to
 * load models and meshes.
 */
[[nodiscard]] Ptr<MeshSource> CreateMeshSource(const AssetArg& filepath);

/**
 * @brief  Creates a new FontSource object from a file path. This can be used to
 * load fonts from a .ttc file.
 */
[[nodiscard]] Ptr<FontSource> CreateFontSource(const AssetArg& filepath);

/**
 * @brief  Finds an existing (loaded) model, and returns a new handle to it.
 */
Pr::Ptr<Pr::ModelAsset> FindModel(const asset_name_t& name);

// TODO: Fix this function to work again with MeshSources
/**
 * @brief Loads models from the disk, and returns a list of loaded models.
 */
/*
Pr::Ptr<Pr::ModelAsset> LoadModel(const AssetArg& filepath,
                   const asset_name_t& customName = "");
                                   */
}  // namespace Pr
