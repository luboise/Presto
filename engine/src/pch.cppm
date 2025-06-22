export module presto.internal.pch;

export import presto.core;
export import presto.math;

export import presto.asset.types;

export {
// Utility stuff
#include <algorithm>  // IWYU pragma: export
#include <chrono>     // IWYU pragma: export
#include <iostream>   // IWYU pragma: export
#include <memory>     // IWYU pragma: export
#include <numeric>    // IWYU pragma: export
#include <ranges>     // IWYU pragma: export
#include <utility>    // IWYU pragma: export
#include <variant>    // IWYU pragma: export

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
}
