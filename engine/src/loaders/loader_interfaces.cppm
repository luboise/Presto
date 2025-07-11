export module presto.internal.loading.loader_interfaces;

import presto.assets;

export import presto.assets.asset;
export import presto.assets.importing;

import presto.types.core;
import std;

export namespace Pr {

class ModelLoader {
   public:
    virtual ~ModelLoader() = default;

    virtual ImportedModelData load(
        const AssetArg& filepath,
        const std::vector<asset_name_t>& customNames) = 0;
};

}  // namespace Pr
