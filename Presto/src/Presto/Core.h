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