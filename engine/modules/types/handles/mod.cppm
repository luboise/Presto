export module presto.types.handles;

import presto.assets;
import presto.types.core;

export namespace Pr {

using MaterialDefinitionPtr = AssetPtr<MaterialAsset>;
using ImagePtr = AssetPtr<ImageAsset>;
using ModelPtr = AssetPtr<ModelAsset>;
using FontPtr = AssetPtr<FontAsset>;

using MaterialPtr = Ptr<MaterialInstance>;
using TexturePtr = Ptr<Texture>;

}  // namespace Pr
