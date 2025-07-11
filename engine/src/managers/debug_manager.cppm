module;
#include "presto/module.h"
#include "presto/platform.h"

export module presto.internal.managers.debug_manager;

import std;

import presto.types.core;
import presto.runtime.window;
import presto.math;

import presto.internal.managers.manager;

export namespace Pr {

struct DebugDrawProps {
    vec4 colour{1, 1, 1, 1};
};

class DebugManager final : public Module<DebugManager> {
    MODULE_FUNCTIONS(DebugManager);

   public:
    ~DebugManager() override;

    void drawAll();
    void update() override;

    explicit DebugManager(Pr::Window*, std::function<void()>);

    void drawLine(vec3 from, vec3 to, DebugDrawProps = {});
    void drawCube(vec3 position, vec3 to, DebugDrawProps = {});
    void drawRect(const Rectangle&, DebugDrawProps = {});

    void setDrawMainCamera(bool enabled);

   private:
    struct Impl;
    Allocated<Impl> impl_;
};

}  // namespace Pr
