#pragma once

#if defined(PR_PLATFORM_WINDOWS)
    #ifdef PR_BUILD_LIB
        #define PRESTO_API __declspec(dllexport)
    #else
        #define PRESTO_API __declspec(dllimport)
    #endif
#elif defined(PR_PLATFORM_UNIX)
    #ifdef PR_BUILD_LIB
        #define PRESTO_API __attribute__((visibility("default")))
    #else
        #define PRESTO_API
    #endif
#else
    #error Build platform not specified. Expected PR_PLATFORM_WINDOWS or PR_PLATFORM_UNIX
#endif

#ifdef PR_ENABLE_ASSERTS
    #define PR_ASSERT(x, ...)                                  \
        {                                                      \
            if (!(x)) {                                        \
                PR_ERROR("Assertion failed: {}", __VA_ARGS__); \
                __debugbreak();                                \
            }                                                  \
        }

    #define PR_CORE_ASSERT(x, ...)                                  \
        {                                                           \
            if (!(x)) {                                             \
                PR_CORE_ERROR("Assertion failed: {}", __VA_ARGS__); \
                __debugbreak();                                     \
            }                                                       \
        }

#else
    #define PR_ASSERT(x, ...)
    #define PR_CORE_ASSERT(x, ...)
#endif

#define AS_BIT(x) (1 << x)

enum PR_RESULT {
    PR_SUCCESS = 0,
    PR_FAILURE = 1,
};