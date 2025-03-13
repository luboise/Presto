#pragma once

#include "Presto/Core.h"
#include "Presto/Events.h"

#include "Presto/Objects/Component.h"

#include "Presto/Platform.h"

namespace Presto {
class Entity;

class PRESTO_API ConductorComponent : public Component {
    friend class EntityManagerImpl;
    friend class EventManager;

   private:
    virtual void start() {};
    virtual void update() {};

    virtual void onInput(KeyEvent& /*unused*/) { handlesInput_ = false; };
    bool handlesInput_{true};

    void onEnterScene() override { this->start(); }

    bool registered_{false};

   protected:
    Entity* entity;

    // Conductor() = default;

   public:
    // ~Conductor() override = default;
};

using Presto::ConductorComponent;
}  // namespace Presto
