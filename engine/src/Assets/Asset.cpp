#include <utility>

#include "Presto/Assets/Asset.h"

namespace Presto {
void Asset::ensureLoaded() {
    if (!loaded_) {
        this->load();
        loaded_ = true;
    }
};

Asset::Asset(PR_STRING_ID name) : name_(std::move(name)) {};

Asset::~Asset() = default;
}  // namespace Presto
