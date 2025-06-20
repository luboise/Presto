export module presto.assets.font;
import presto.assets;

export namespace Presto {
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
