#pragma once

#include "Presto/Rendering/MaterialData.h"
#include "Presto/Resources/Image.h"

#include "Presto/Resources/Resource.h"

#include "Presto/Resources/ImageResource.h"

namespace Presto {
    class MaterialResource final : public Resource {
       public:
        friend class RenderingManager;

        explicit MaterialResource(PR_STRING_ID name,
                                  ImageResource* image = nullptr);
        ~MaterialResource() override = default;

        [[nodiscard]] constexpr ResourceType getType() const override {
            return ResourceType::MATERIAL;
        };

        [[nodiscard]] MaterialData getData() const;

        void setImage(ImageResource* image);

        [[nodiscard]] Image getImage() const;

       private:
        void load() override;

        ImageResource* diffuseImage_;
    };
}  // namespace Presto
