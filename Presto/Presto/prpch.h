#pragma once

// Presto Includes
#include "Presto/Core.h"
#include "Presto/Log.h"

// External includes
#include <glm/glm.hpp>

// Utility stuff
#include <functional>
#include <iostream>
#include <memory>
#include <utility>

// Data
#include <sstream>
#include <string>
#include <vector>

// Multi threading
#include <atomic>
#include <thread>

#ifdef PR_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <Xinput.h>
#endif