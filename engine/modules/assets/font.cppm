export module presto.assets.font;
import presto.assets.types;
import presto.core.types;

export namespace Pr {
class FontAsset final : public Asset {
    friend class RenderingManager;

   public:
    explicit FontAsset(Pr::string name);

    ~FontAsset() override = default;

    [[nodiscard]] constexpr AssetType type() const override {
        return AssetType::FONT;
    };

   private:
    bool load() override;
};
