#pragma once

#include "Entity/Entity.h"
#include "Presto/Modules/Module.h"

namespace Presto {
    class PRESTO_API EntityManager : public Module {
       public:
        static entity_t newEntity();
    };
}  // namespace Presto