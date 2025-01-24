#include <utility>

#include "Presto/Resources/Resource.h"

namespace Presto {
    void Resource::ensureLoaded() {
        if (!loaded_) {
            this->load();
            loaded_ = true;
        }
    };

    Resource::Resource(PR_STRING_ID name) : name_(std::move(name)) {};
}  // namespace Presto
