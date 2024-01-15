#pragma once

// Presto Includes
#include "Presto/Log.h"

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

#ifdef HZ_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <Xinput.h>
#endif