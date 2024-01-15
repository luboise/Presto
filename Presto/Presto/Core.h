#pragma once

#ifdef PR_PLATFORM_WINDOWS
    #ifdef PR_BUILD_DLL
        #define PRESTO_API __declspec(dllexport)
    #else
        #define PRESTO_API __declspec(dllimport)
    #endif
#else
    #error Windows!
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