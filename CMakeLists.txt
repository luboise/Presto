cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS Off)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/ext")

find_program(
  GLSLC_PATH
  NAMES glslc
  HINTS Vulkan::glslc)

project(Presto)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

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

# add_subdirectory(ext/tinyobjloader)

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

if(WIN32)
  set(PLATFORM Windows)

  add_compile_definitions(PR_PLATFORM_WINDOWS)
  add_compile_definitions(NOMINMAX)

  set(CMAKE_GENERATOR_PLATFORM
      "x64"
      CACHE STRING "Platform (x64)" FORCE)
elseif(UNIX)
  set(PLATFORM Unix)
  add_compile_definitions(PR_PLATFORM_UNIX)
else()
  message(FATAL_ERROR "Platform not selected."
                      "Expected Windows or Unix platform for compilation.")
endif()

# Engine definitions

add_library(PrestoEngine SHARED)

target_include_directories(PrestoEngine
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(PrestoEngine PUBLIC ../ext/spdlog/include
                                               ../ext/glm/include)
target_include_directories(
  PrestoEngine PRIVATE ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS}
                       ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(PrestoEngine PUBLIC nlohmann_json::nlohmann_json)
target_link_libraries(PrestoEngine PRIVATE glm::glm)

# target_link_libraries(PrestoEngine PRIVATE glfw)
if(PLATFORM STREQUAL "Windows")
  target_link_libraries(PrestoEngine PRIVATE dwmapi.lib XInput9_1_0.lib)
endif()

# Needed for shared library
set_property(TARGET PrestoEngine PROPERTY POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(PrestoEngine PRIVATE PR_BUILD_LIB)

add_subdirectory(src)

# target_link_libraries(PrestoEngine PRIVATE glfw)
if(PLATFORM STREQUAL "Windows")
  target_link_libraries(PrestoEngine PRIVATE dwmapi.lib XInput9_1_0.lib)
endif()

target_link_libraries(PrestoEngine PUBLIC glfw ${Vulkan_LIBRARIES} spdlog)

set(TINYGLTF_HEADER_ONLY
    OFF
    CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL
    OFF
    CACHE INTERNAL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ext/TinyGLTF)

# target_include_directories(PrestoEngine PUBLIC
# ${CMAKE_CURRENT_SOURCE_DIR}/ext/TinyGLTF)

target_link_libraries(PrestoEngine PRIVATE ${OPENGL_LIBRARIES} glfw GLEW::GLEW)
target_link_libraries(PrestoEngine PRIVATE tinygltf)

# Needed for shared library
set_property(TARGET PrestoEngine PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET tinygltf PROPERTY POSITION_INDEPENDENT_CODE ON)

set(PRESTO_BASE_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders)
compileshadersfortarget(PrestoEngine "${PRESTO_BASE_SHADER_DIR}")
