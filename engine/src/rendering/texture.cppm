export module presto.internal.rendering:texture;

import presto.internal;

export namespace Presto {

// TODO: Implement other texture types here
enum class TextureType : Presto::uint8_t { TEX2D };

class PRESTO_API Texture {
   public:
    virtual void bind(Presto::uint8_t slot) = 0;
    virtual void load() = 0;

    [[nodiscard]] texture_id_t id() const { return id_; }
    [[nodiscard]] bool loaded() const { return id_ == UNLOADED_TEXTURE_ID; }

    virtual ~Texture() = default;

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

   protected:
    void setId(texture_id_t id) { id_ = id; }

    static constexpr texture_id_t UNLOADED_TEXTURE_ID{
        static_cast<texture_id_t>(-1U)};

    Texture() = default;

   private:
    texture_id_t id_{UNLOADED_TEXTURE_ID};
};

class Texture2D : public Texture {
   public:
    virtual void setImage(const ImagePtr& ptr) = 0;

    virtual void write(ByteArray bytes) = 0;

    [[nodiscard]] ImagePtr image() const { return image_; }

    [[nodiscard]] virtual std::size_t width() const = 0;
    [[nodiscard]] virtual std::size_t height() const = 0;
    [[nodiscard]] std::size_t pixelCount() const { return width() * height(); };

   private:
    ImagePtr image_;
};

template <TextureType T>
struct TextureTypeDetails {};

template <>
struct TextureTypeDetails<TextureType::TEX2D> {
    using TexInterface = Texture2D;
};

template <TextureType T>
    requires requires { typename TextureTypeDetails<T>; }
using TexInterfaceOf = TextureTypeDetails<T>::TexInterface;

class TextureFactory {
   protected:
    using texture_extents_t = Presto::size_t;

   public:
    template <typename T>
    using FactoryOutputType = Ptr<T>;

    virtual ~TextureFactory() = default;

    [[nodiscard]] virtual FactoryOutputType<Texture2D> new2D(
        texture_extents_t width, texture_extents_t height) = 0;
};

}  // namespace Presto
