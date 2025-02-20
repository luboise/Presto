#include "ModelLoader.h"

namespace Presto {

class GLTFLoader : public ModelLoader {
   public:
    ImportedModelData load(
        const AssetArg& filepath,
        const std::vector<asset_name_t>& customNames) override;
};

}  // namespace Presto
