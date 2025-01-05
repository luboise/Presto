#pragma once

#include <cstdint>

#include "Presto/Platform.h"

using component_class_t = uint32_t;

namespace Presto {
    class PRESTO_API Component {
       public:
        Component() = default;
        virtual ~Component() = default;

        [[nodiscard]] component_class_t getClassID() const;

        template <typename T>
        bool isOfType() {
            return dynamic_cast<T>(this) != nullptr;
        }

       private:
        bool renderable_ = false;
    };

    using component_ptr = Component*;
}  // namespace Presto
