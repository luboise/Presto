#include "Presto/Rendering/TextureTypes.h"

namespace Presto {

class TextureFactory {
    template <typename T>
    using FactoryOutputType = Ptr<T>;

   protected:
    using texture_extents_t = Presto::size_t;

   public:
    virtual ~TextureFactory() = default;

    [[nodiscard]] virtual FactoryOutputType<Texture2D> new2D(
        texture_extents_t width, texture_extents_t height) = 0;
};

}  // namespace Presto
