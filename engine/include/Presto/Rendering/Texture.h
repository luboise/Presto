#pragma once
#include "Presto/Core/Types.h"
namespace Presto {

class PRESTO_API Texture {
   public:
    virtual void bind(Presto::uint8_t slot) = 0;

    Texture() = default;
    virtual ~Texture() = default;

    Texture(const Texture&) = delete;
    Texture(Texture&&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

   private:
};

}  // namespace Presto
