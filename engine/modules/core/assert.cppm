module;
#include <csignal>

export module presto.core.assert;

import std;

import presto.core.logging;
import presto.types.core;

#ifndef NDEBUG
#if defined(PR_PLATFORM_WINDOWS)
#define TRIP_DEBUGGER() __debugbreak()
#elif defined(PR_PLATFORM_UNIX)
#if defined(SIGTRAP)
#define TRIP_DEBUGGER() raise(SIGTRAP)
#else
#define TRIP_DEBUGGER() raise(SIGABRT)
#endif
#endif

inline constexpr bool ASSERTIONS_ENABLED =
#ifdef DEBUG
    true;
#else
    false;
#endif

#else
// Fallback for release builds
#define TRIP_DEBUGGER() ((void)0)
inline constexpr bool ASSERTIONS_ENABLED = false;
#endif

export namespace Pr {

inline void Assert(bool condition, Pr::string message) {
    if constexpr (ASSERTIONS_ENABLED) {
        if (!condition) {
            Pr::Log(ERROR, message);
            TRIP_DEBUGGER();
        }
    }
}

// TODO: Move to private fragment
inline void CoreAssert(bool condition, Pr::string message) {
    if constexpr (ASSERTIONS_ENABLED) {
        if (!condition) {
            Pr::CoreLog(ERROR, message);
            TRIP_DEBUGGER();
        }
    }
}

}  // namespace Pr
