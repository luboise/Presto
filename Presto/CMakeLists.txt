add_library(Presto SHARED)

if (WIN32)
	target_compile_definitions(Presto
		PRIVATE PR_BUILD_DLL
	)
endif()

target_precompile_headers(Presto PRIVATE Presto/prpch.h)

target_include_directories(Presto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Presto PUBLIC include)

target_sources(Presto PRIVATE include/Presto.h)

add_subdirectory(Presto)
