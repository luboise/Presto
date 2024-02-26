#pragma once

// Presto Includes
#include "Presto/Core.h"
#include "Presto/Log.h"

// Utility stuff
#include <iostream>
#include <memory>
#include <utility>

// Data
#include <sstream>
#include <string>
#include <vector>

// GLM
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Multi threading
#include <atomic>
#include <thread>

#ifdef PR_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <Xinput.h>
#endif