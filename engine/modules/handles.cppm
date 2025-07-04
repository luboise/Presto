export module presto.handles;

import presto.assets;

export namespace Pr {

using MaterialDefinitionPtr = AssetPtr<MaterialAsset>;
using ImagePtr = AssetPtr<ImageAsset>;
using ModelPtr = AssetPtr<ModelAsset>;
using FontPtr = AssetPtr<FontAsset>;
using MeshPtr = AssetPtr<Mesh>;

using MaterialPtr = Ptr<MaterialInstance>;
using TexturePtr = Ptr<Texture>;

}  // namespace Pr
