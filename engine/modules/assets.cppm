export module presto.assets;

export import presto.assets.types;
export import presto.assets.image;
export import presto.assets.material;
export import presto.assets.model;
export import presto.assets.types;

export namespace Presto {

// Materials
MaterialPtr NewMaterial(MaterialType type, Presto::string name = "");
MaterialPtr FindMaterial(const Presto::string& name);

// Textures
/**
 * @brief  Creates a new texture based on a filepath. This is the equivalent to
 * loading the image first to obtain an ImagePtr, then calling
 * loadTexture2D(ptr)
 */
Ptr<Texture2D> NewTexture2D(AssetArg path);
Ptr<Texture2D> NewTexture2D(const ImagePtr& ptr);

}  // namespace Presto
