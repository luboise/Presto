#pragma once

// Utility stuff
#include <algorithm>  // IWYU pragma: export
#include <iostream>   // IWYU pragma: export
#include <memory>     // IWYU pragma: export
#include <ranges>     // IWYU pragma: export
#include <utility>    // IWYU pragma: export

// Data
#include <fstream>  // IWYU pragma: export
#include <sstream>  // IWYU pragma: export
#include <string>   // IWYU pragma: export
#include <vector>   // IWYU pragma: export

// Program Control
#include <stdexcept>  // IWYU pragma: export

#ifdef PR_PLATFORM_WINDOWS
#include <Windows.h>
#include <Xinput.h>
#endif

#include "Presto/Platform.h"  // IWYU pragma: export
#include "Utils/Math.h"       // IWYU pragma: export

#include "Presto/Core/Assert.h"     // IWYU pragma: export
#include "Presto/Core/Constants.h"  // IWYU pragma: export
#include "Presto/Core/Logging.h"    // IWYU pragma: export
