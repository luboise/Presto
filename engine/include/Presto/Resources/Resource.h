#pragma once

#include "Presto/Core/Concepts.h"

namespace Presto {
    enum class ResourceType { MESH, MATERIAL, IMAGE };

    class Resource {
        friend class ResourceManager;

       public:
        explicit Resource(PR_STRING_ID name);
        virtual ~Resource();

        [[nodiscard]] constexpr virtual ResourceType getType() const = 0;

        void ensureLoaded();

        [[nodiscard]] bool loaded() const { return loaded_; }

        [[nodiscard]] PR_STRING_ID name() const { return name_; }

        template <DerivedFrom<Resource> Subclass>
        Subclass* as() {
            return dynamic_cast<Subclass*>(this);
        }

       private:
        virtual void load() = 0;

        PR_STRING_ID name_;

        bool loaded_{false};
    };

}  // namespace Presto
