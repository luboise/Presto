#pragma once

#include "Presto/Core/Concepts.h"

namespace Presto {
    enum class AssetType { MESH, MODEL, MATERIAL, IMAGE };

    class Asset {
        friend class AssetManager;

       public:
        explicit Asset(asset_name_t name = PR_ANY_NAME);
        virtual ~Asset();

        [[nodiscard]] constexpr virtual AssetType getType() const = 0;

        void ensureLoaded();

        [[nodiscard]] bool loaded() const { return loaded_; }

        [[nodiscard]] asset_name_t name() const { return name_; }

        template <DerivedFrom<Asset> Subclass>
        Subclass* as() {
            return dynamic_cast<Subclass*>(this);
        }

       private:
        virtual void load() = 0;

        asset_name_t name_;

        bool loaded_{false};
    };

    template <DerivedFrom<Asset> T>
    using AssetPtr = Ptr<T>;
}  // namespace Presto
