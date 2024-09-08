#pragma once

#include <cstdint>
#include <map>

typedef uint32_t component_class_t;

namespace Presto {
    class PRESTO_API Component {
       public:
        Component() {}
        virtual ~Component() {};
        component_class_t getClassID() const;
    };
}  // namespace Presto
