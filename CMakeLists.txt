cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

project(Presto)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

if (WIN32)
	add_compile_definitions(PR_PLATFORM_WINDOWS)
	add_compile_definitions(NOMINMAX)
endif()

set(CMAKE_GENERATOR_PLATFORM "x64"
	CACHE STRING "Platform (x64)" FORCE
)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "Build Configurations" FORCE)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path" FORCE)
endif()

add_subdirectory(ext/spdlog)

add_subdirectory(ext/GLFW)
link_libraries(glfw)

add_subdirectory(ext/glm)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

add_subdirectory(Presto)
target_link_libraries(Presto
	PRIVATE glfw glm ${Vulkan_LIBRARIES} opengl32.lib dwmapi.lib XInput9_1_0.lib
	PUBLIC spdlog
)
# target_link_libraries(Presto PRIVATE glfw)

add_subdirectory(Duality)

# add_subdirectory(PrestoTest)

