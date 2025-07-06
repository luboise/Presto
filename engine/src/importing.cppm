export module presto.internal.importing;

import presto.types.core;

export namespace Pr {

class ModelLoader {
   public:
    virtual ~ModelLoader() = default;

    virtual ImportedModelData load(
        const AssetArg& filepath,
        const std::vector<asset_name_t>& customNames) = 0;
};

}  // namespace Pr
