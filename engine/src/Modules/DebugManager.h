#pragma once

#include "Modules/Module.h"

namespace Presto {

class Window;

class DebugManager final : public Module<DebugManager> {
    MODULE_FUNCTIONS(DebugManager);

   public:
    void update() override;

    explicit DebugManager(Presto::Window*, std::function<void()>);

    void drawLine(vec3 from, vec3 to, vec4 colour);

   private:
    struct Impl;
    Allocated<Impl> impl_;
};

}  // namespace Presto
