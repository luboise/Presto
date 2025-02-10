#pragma once

#include "Presto/Assets/ImageAsset.h"
#include "Presto/Assets/MaterialInstanceAsset.h"

namespace Presto {

class PBRMaterial : public MaterialInstanceAsset {
   public:

   private:
    struct {
        Presto::vec4 colour{1, 1, 1, 1};
        Presto::ImagePtr diffuse_texture{nullptr};

        Presto::float32_t metallic{0};
        Presto::float32_t rough{0};

        Presto::ImagePtr normal_map{nullptr};
        Presto::ImagePtr bump_map{nullptr};

        Presto::float32_t specular;
        Presto::vec3 specular_tint{1, 1, 1};

        Presto::ImagePtr emission_map{nullptr};
    } properties_;
};

}  // namespace Presto
