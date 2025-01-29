#pragma once

#include "Presto/Core.h"
#include "Presto/Events.h"
#include "Presto/Objects.h"
#include "Presto/Platform.h"

namespace Presto {
    class PRESTO_API ConductorComponent : public Component {
        friend class EntityManager;
        friend class EventManager;

       private:
        virtual void start() {};
        virtual void update() {};

        virtual void onInput(KeyEvent& /*unused*/) { handlesInput_ = false; };
        bool handlesInput_{true};

        void onEnterScene() override { this->start(); }

       protected:
        Entity* entity{nullptr};

        // Conductor() = default;

       public:
        // ~Conductor() override = default;
    };

    using Presto::ConductorComponent;
}  // namespace Presto
