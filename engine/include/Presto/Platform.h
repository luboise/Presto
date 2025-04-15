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

#ifdef PR_DEBUG_BUILD
#define PR_DEBUG_ONLY_CODE(x) x;
#else
#define PR_DEBUG_ONLY_CODE(x)
#endif
