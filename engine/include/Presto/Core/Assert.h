#pragma once

#include "Presto/Core/Logging.h"

// Mandatory libraries
// #include <cstdint>

#ifndef NDEBUG
#if defined(PR_PLATFORM_WINDOWS)
#define TRIP_DEBUGGER() __debugbreak()
#elif defined(PR_PLATFORM_UNIX)
#include <csignal>  // IWYU pragma: export
#include <cstdio>   // IWYU pragma: export

#if defined(SIGTRAP)
#define TRIP_DEBUGGER() raise(SIGTRAP)
#else
#define TRIP_DEBUGGER() raise(SIGABRT)
#endif
#endif

#define PR_ASSERT(x, ...)                                  \
    {                                                      \
        if (!(x)) {                                        \
            PR_ERROR("Assertion failed: {}", __VA_ARGS__); \
            TRIP_DEBUGGER();                               \
        }                                                  \
    }

#define PR_CORE_ASSERT(x, ...)                                  \
    {                                                           \
        if (!(x)) {                                             \
            PR_CORE_ERROR("Assertion failed: {}", __VA_ARGS__); \
            TRIP_DEBUGGER();                                    \
        }                                                       \
    }

#else
#define PR_ASSERT(x, ...)
#define PR_CORE_ASSERT(x, ...)
#endif
