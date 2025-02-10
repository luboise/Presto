#pragma once

#include "Presto/Core/Constants.h"

namespace Presto {
class Renderer;
class GLFWAppWindow;

Allocated<Renderer> CreateRenderer(RENDER_LIBRARY lib, GLFWAppWindow* window);
}  // namespace Presto
