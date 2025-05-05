#pragma once

#include "Modules/Module.h"

namespace Presto {

class Window;
struct Rectangle;

struct DebugDrawProps {
    vec4 colour{1, 1, 1, 1};
};

class DebugManager final : public Module<DebugManager> {
    MODULE_FUNCTIONS(DebugManager);

   public:
    ~DebugManager() override;

    void drawAll();
    void update() override;

    explicit DebugManager(Presto::Window*, std::function<void()>);

    void drawLine(vec3 from, vec3 to, vec4 colour);
    void drawCube(vec3 position, vec3 to, vec4 colour);
    void drawRect(const Rectangle&, DebugDrawProps = {});

    void setDrawMainCamera(bool enabled);

   private:
    struct Impl;
    Allocated<Impl> impl_;
};

}  // namespace Presto
