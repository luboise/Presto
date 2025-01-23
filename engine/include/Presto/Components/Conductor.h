#pragma once

#include "Presto/Core.h"

#include "Presto/Platform.h"

#include "Presto/Events.h"

#include "Presto/Objects.h"

namespace Presto {
    class PRESTO_API Conductor : public Component {
        friend class EntityManager;
        friend class EventManager;

       private:
        virtual void start() {};
        virtual void update() {};

        virtual void onInput(KeyEvent& event) { handlesInput_ = false; };
        bool handlesInput_{true};

       protected:
        Entity* entity{nullptr};

        // Conductor() = default;

       public:
        // ~Conductor() override = default;
    };

    using Presto::Conductor;
}  // namespace Presto
