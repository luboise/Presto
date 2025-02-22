#include <utility>

#include "Presto/Assets/Asset.h"

namespace Presto {
bool Asset::ensureLoaded() {
    if (!loaded_) {
        loaded_ = this->load();

        if (!loaded_) {
            PR_ERROR("Unable to load asset \"{}\" of type {}", this->name(),
                     static_cast<Presto::uint8_t>(this->type()));
        }
    }

    return loaded_;
};

Asset::Asset(PR_STRING_ID name) : name_(std::move(name)) {};

Asset::~Asset() = default;
}  // namespace Presto
