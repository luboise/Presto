#pragma once

#include "Presto/Core/Concepts.h"

namespace Presto {
    enum class AssetType { MESH, MATERIAL, IMAGE };

    class Asset {
        friend class ResourceManager;

       public:
        explicit Asset(PR_STRING_ID name);
        virtual ~Asset();

        [[nodiscard]] constexpr virtual AssetType getType() const = 0;

        void ensureLoaded();

        [[nodiscard]] bool loaded() const { return loaded_; }

        [[nodiscard]] PR_STRING_ID name() const { return name_; }

        template <DerivedFrom<Asset> Subclass>
        Subclass* as() {
            return dynamic_cast<Subclass*>(this);
        }

       private:
        virtual void load() = 0;

        PR_STRING_ID name_;

        bool loaded_{false};
    };

}  // namespace Presto
