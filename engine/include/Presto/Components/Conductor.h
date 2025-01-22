#pragma once

#include "Presto/Core.h"

#include "Presto/Platform.h"

namespace Presto {
    class PRESTO_API Conductor : public Component {
        friend class EntityManager;

       private:
        virtual void start(Entity& e) {};
        virtual void update(Entity& e) {};
        // Conductor() = default;

       public:
        // ~Conductor() override = default;
    };

    using Presto::Conductor;
}  // namespace Presto
