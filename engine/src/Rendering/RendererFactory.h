#pragma once

#include "Presto/Core/Constants.h"

namespace Presto {
class Renderer;
class GLFWAppWindow;

Allocated<Renderer> CreateRenderer();
}  // namespace Presto
