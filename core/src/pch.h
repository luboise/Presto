#pragma once

// Presto Includes
// #include "PrestoCore/Presto.h"  // IWYU pragma: export

// Utility stuff
#include <iostream>  // IWYU pragma: export
#include <memory>    // IWYU pragma: export
#include <utility>   // IWYU pragma: export

// Data
#include <sstream>  // IWYU pragma: export
#include <string>   // IWYU pragma: export
#include <vector>   // IWYU pragma: export

// Multi threading
#include <atomic>  // IWYU pragma: export
#include <thread>  // IWYU pragma: export

#ifdef PR_PLATFORM_WINDOWS
    #include <Windows.h>
    #include <Xinput.h>
#endif

#include "PrestoCore/Core.h"
#include "PrestoCore/Logging.h"  // IWYU pragma: export
#include "PrestoCore/Math.h"     // IWYU pragma: export
