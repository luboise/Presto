#pragma once

#include <utility>

#include "Presto/Assets/AssetPath.h"

namespace Presto {

class AssetSource {
   public:
    virtual ~AssetSource() { this->ensureUnloaded(); }

    virtual void unload() = 0;

    [[nodiscard]] bool loaded() const { return loaded_; }
    [[nodiscard]] AssetPath path() const { return path_; }

    virtual void setPath(AssetPath path) {
        this->ensureUnloaded();
        path_ = std::move(path);
    };

    AssetSource(const AssetSource&) = default;
    AssetSource(AssetSource&&) = delete;
    AssetSource& operator=(const AssetSource&) = default;
    AssetSource& operator=(AssetSource&&) = delete;

   protected:
    AssetSource() : path_(""), loaded_(false) {}

    void ensureUnloaded() {
        // if (loaded_) {
            // this->unload();
        // }
    }

    explicit AssetSource(AssetPath path, bool loaded)
        : path_(std::move(path)), loaded_(loaded) {}

   private:
    AssetPath path_;
    bool loaded_;
};

}  // namespace Presto
