#pragma once

#include "PrestoCore/Core/Constants.h"
#include "PrestoCore/Rendering/Renderer.h"

namespace PrestoCore {
    Renderer* CreateRenderer(RENDER_LIBRARY lib, GLFWAppWindow* window);
}
