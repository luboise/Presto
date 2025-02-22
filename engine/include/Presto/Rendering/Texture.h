#pragma once

#include "Presto/Core/Constants.h"
#include "Presto/Core/Types.h"

namespace Presto {

// TODO: Implement other texture types here
enum class TextureType : Presto::uint8_t { RECT };

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

using TexturePtr = Ptr<Texture>;

}  // namespace Presto
