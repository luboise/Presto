
#if defined(PR_PLATFORM_WINDOWS)
    #ifdef PR_BUILD_LIB
        #define PRESTO_API __declspec(dllexport)
    #else
        #define PRESTO_API __declspec(dllimport)
    #endif
#elif defined(PR_PLATFORM_UNIX)
    #include <csignal>  // IWYU pragma: export
    #include <cstdio>   // IWYU pragma: export

    #ifdef PR_BUILD_LIB
        #define PRESTO_API __attribute__((visibility("default")))
    #else
        #define PRESTO_API
    #endif
#else
    #error Build platform not specified. Expected PR_PLATFORM_WINDOWS or PR_PLATFORM_UNIX
#endif
