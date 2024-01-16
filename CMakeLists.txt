cmake_minimum_required(VERSION 3.28)
project(Presto)


if (WIN32)
	add_compile_definitions(PR_PLATFORM_WINDOWS)
	add_compile_definitions(PR_BUILD_DLL)
endif()

set(CMAKE_GENERATOR_PLATFORM "x64"
	CACHE STRING "Platform (x64)" FORCE
)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "Build Configurations" FORCE)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path" FORCE)
endif()

# Ensure x64
# set(CMAKE_GENERATOR_PLATFORM x64 CACHE STRING "Platform (Win32 or x64)" FORCE)

add_subdirectory(ext/spdlog)
# target_include_directories(spdlog INTERFACE ${spdlog_SOURCE_DIR}/include)

add_subdirectory(ext/GLFW)
link_libraries(glfw)

add_subdirectory(Presto)
target_link_libraries(Presto PUBLIC spdlog glfw opengl32.lib dwmapi.lib XInput9_1_0.lib)
# target_link_libraries(Presto PUBLIC glfw)