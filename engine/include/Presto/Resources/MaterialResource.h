#pragma once

#include "Presto/Resources/Image.h"

#include "Presto/Resources/Resource.h"

#include "Presto/Resources/ImageResource.h"

namespace Presto {
    class MaterialResource final : public Resource {
       public:
        MaterialResource(PR_STRING_ID name, ImageResource* image);

        [[nodiscard]] constexpr ResourceType getType() const override {
            return ResourceType::MATERIAL;
        };

        void setImage(ImageResource* image);

        [[nodiscard]] Image getImage() const;

       private:
        void load() override;

        ImageResource* image_;
    };
}  // namespace Presto
