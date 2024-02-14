add_library(Presto SHARED)
# Needed for shared library
set_property(TARGET Presto PROPERTY POSITION_INDEPENDENT_CODE ON)


# if (WIN32)
# 	target_compile_definitions(Presto
# 		PRIVATE PR_BUILD_LIB
# 	)
# endif()

target_compile_definitions(Presto
	PRIVATE PR_BUILD_LIB
)

target_precompile_headers(Presto PRIVATE Presto/prpch.h)

target_include_directories(Presto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(Presto PUBLIC include)

target_sources(Presto PRIVATE include/Presto.h)

add_subdirectory(Presto)

include(${CMAKE_SOURCE_DIR}/cmake_build_shaders.cmake)

# Set the source and build directories
CompileShadersForTarget(Presto ${CMAKE_CURRENT_SOURCE_DIR}/Presto/Shaders)