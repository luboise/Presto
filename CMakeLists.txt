cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/ext")

find_program(
  GLSLC_PATH
  NAMES glslc
  HINTS Vulkan::glslc)

project(Presto)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

set(CMAKE_CONFIGURATION_TYPES
    "Debug;Release;Dist"
    CACHE STRING "Build Configurations" FORCE)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      "${CMAKE_BINARY_DIR}/install"
      CACHE PATH "Install path" FORCE)
endif()

if(NOT TARGET spdlog)
  # find_package(spdlog REQUIRED)
  add_subdirectory(ext/spdlog)
endif()
set_property(TARGET spdlog PROPERTY POSITION_INDEPENDENT_CODE ON)

add_subdirectory(ext/tinyobjloader)

set(JSON_BuildTests
    OFF
    CACHE INTERNAL "")
add_subdirectory(ext/json)

# set(glm_DIR ./ext/glm) find_package(glm REQUIRED)

# OpenGL Libraries
find_package(OpenGL REQUIRED)
add_subdirectory(ext/GLFW)
find_package(GLEW REQUIRED)

add_subdirectory(ext/glm)
# find_package(glm REQUIRED PATHS ./ext/glm)

find_package(Vulkan REQUIRED)
# include_directories(${Vulkan_INCLUDE_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/core)
