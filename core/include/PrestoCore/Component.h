#pragma once

#include <cstdint>

using component_class_t = uint32_t;

namespace Presto {
    class PRESTO_API Component {
       public:
        Component() {}
        virtual ~Component() = default;
        [[nodiscard]] component_class_t getClassID() const;
    };
}  // namespace Presto
