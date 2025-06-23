#include "presto/assets/types.h"
#include "presto/core/types.h"

namespace Presto {
class FontAsset final : public Asset {
    friend class RenderingManager;

   public:
    explicit FontAsset(Presto::string name);

    ~FontAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::FONT;
    };

   private:
    bool load() override;
};
