#pragma once

// Presto Includes
#include "PrestoCore/Presto.h"  // IWYU pragma: export

// Utility stuff
#include <iostream>  // IWYU pragma: export
#include <memory>    // IWYU pragma: export
#include <utility>   // IWYU pragma: export

// Data
#include <sstream>  // IWYU pragma: export
#include <string>   // IWYU pragma: export
#include <vector>   // IWYU pragma: export

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Multi threading
#include <atomic>  // IWYU pragma: export
#include <thread>  // IWYU pragma: export

#ifdef PR_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <Xinput.h>
#endif
