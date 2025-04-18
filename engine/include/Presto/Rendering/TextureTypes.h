#pragma once

#include "Presto/Core/Constants.h"
#include "Presto/Types/CoreTypes.h"

namespace Presto {

// TODO: Implement other texture types here
enum class TextureType : Presto::uint8_t { TEX2D };

class PRESTO_API Texture {
    using texture_id_t = PR_NUMERIC_ID;
    static constexpr texture_id_t TEXTURE_ID{-1U};

   public:
    virtual void bind(Presto::uint8_t slot) = 0;

    virtual ~Texture() = default;

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    [[nodiscard]] texture_id_t id() const { return id_; }

   protected:
    Texture() = default;

    void setId(texture_id_t id) { id_ = id; }

   private:
    texture_id_t id_{};
};

class Texture2D : public Texture {
   public:
    [[nodiscard]] virtual std::size_t width() const = 0;
    [[nodiscard]] virtual std::size_t height() const = 0;

    virtual void write(ByteArray bytes) = 0;

    [[nodiscard]] std::size_t pixelCount() const { return width() * height(); };
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

}  // namespace Presto
