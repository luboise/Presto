export module presto.internal.loading.gltf_loader;

import presto.internal.loading.loader_interfaces;
import std;

export namespace Pr {

class GLTFLoader : public ModelLoader {
   public:
    ImportedModelData load(
        const AssetArg& filepath,
        const std::vector<asset_name_t>& customNames = {}) override;
};

}  // namespace Pr
