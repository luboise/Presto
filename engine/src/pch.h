#pragma once

// Utility stuff
#include <algorithm>  // IWYU pragma: export
#include <iostream>   // IWYU pragma: export
#include <memory>     // IWYU pragma: export
#include <utility>    // IWYU pragma: export

// Data
#include <fstream>  // IWYU pragma: export
#include <sstream>  // IWYU pragma: export
#include <string>   // IWYU pragma: export
#include <vector>   // IWYU pragma: export

// Program Control
#include <stdexcept>  // IWYU pragma: export

// Multi-Threading
#include <atomic>  // IWYU pragma: export
#include <thread>  // IWYU pragma: export

#ifdef PR_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <Xinput.h>
#endif

#include "Presto/Core.h"      // IWYU pragma: export
#include "Presto/Objects.h"   // IWYU pragma: export
#include "Presto/Platform.h"  // IWYU pragma: export
#include "Utils/Math.h"       // IWYU pragma: export

#include "Presto/Utils.h"  // IWYU pragma: export

// Object includes
// #include "Presto/Rendering/RenderingManager.h"  // IWYU pragma: export

// Type includes
// #include "Rendering/Vulkan/VulkanVertex.h"  // IWYU pragma: export

// ENTRY POINT - NEED TO ENTER THIS YOURSELF
// #include "Presto/EntryPoint.h"  // IWYU pragma: export

// Presto Includes
// #include "Presto/Presto.h"  // IWYU pragma: export
