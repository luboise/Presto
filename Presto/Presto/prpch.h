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

// Maths
#include <glm/glm.hpp>

// Multi threading
#include <atomic>
#include <thread>

#ifdef PR_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <Xinput.h>
#endif