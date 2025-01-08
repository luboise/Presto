#pragma once

#include "Presto/Core/Constants.h"
#include "Presto/Rendering/Renderer.h"

namespace Presto {
    Renderer* CreateRenderer(RENDER_LIBRARY lib, GLFWAppWindow* window);
}
