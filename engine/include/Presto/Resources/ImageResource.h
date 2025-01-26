#pragma once

#include <utility>

#include "Presto/Resources/Image.h"

#include "Presto/Resources/Resource.h"

// #include "Presto/Rendering/Renderer.h"

namespace Presto {
    class ImageResource final : public Resource {
        using image_data_t = std::vector<uint8_t>;

       public:
        ImageResource(PR_STRING_ID name, Presto::Image image)
            : Resource(std::move(name)), image_(std::move(image)) {}

        [[nodiscard]] constexpr ResourceType getType() const override {
            return ResourceType::IMAGE;
        };

        [[nodiscard]] Presto::Image getImage() const { return image_; }

        [[nodiscard]] std::size_t getWidth() const { return image_.width; }
        [[nodiscard]] std::size_t getHeight() const { return image_.height; }
        [[nodiscard]] const image_data_t& getData() const {
            return image_.bytes;
        }

        // 4 bytes since assumed RGBA, 1 for each channel
        [[nodiscard]] std::size_t size() const { return image_.size(); };

       private:
        void load() override;

        Presto::Image image_;
    };
}  // namespace Presto
