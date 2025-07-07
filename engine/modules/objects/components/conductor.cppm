module;
#include "presto/platform.h"

export module presto.objects.components.conductor;

import std;

import presto.objects.base;
import presto.objects.base;

export namespace Pr {

class PRESTO_API ConductorComponent : public Component {
    friend class Entity;

   private:
    using pre_start_callback_t = std::function<void()>;

    virtual void start() {};
    virtual void update() {};

    // virtual void on(KeyEvent& /*unused*/) { handlesKeyEvents_ = false; };
    // bool handlesKeyEvents_{true};

    void onEnterScene() override;

    bool registered_{false};

    std::list<pre_start_callback_t> preStartCallbacks_;

   protected:
    Entity* entity;

    void addPreStartCallback(const pre_start_callback_t& callback);

    // Conductor() = default;

   public:
    // ~Conductor() override = default;
};

}  // namespace Pr
