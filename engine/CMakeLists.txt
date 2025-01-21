project(Presto)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

# find_package(glm REQUIRED PATHS ./ext/glm)

# Engine definitions
add_library(PrestoEngine SHARED)

target_include_directories(PrestoEngine
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(
  PrestoEngine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ext/spdlog/include
                      ${CMAKE_CURRENT_SOURCE_DIR}/../ext/glm/include)

target_include_directories(
  PrestoEngine
  PRIVATE ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS}
          ${CMAKE_CURRENT_SOURCE_DIR}/src
          ${CMAKE_CURRENT_SOURCE_DIR}/../ext/stb/)

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

set(PRESTO_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

# target_include_directories(PrestoEngine PUBLIC
# ${CMAKE_CURRENT_SOURCE_DIR}/ext/TinyGLTF)

target_link_libraries(PrestoEngine PRIVATE ${OPENGL_LIBRARIES} glfw GLEW::GLEW)
target_link_libraries(PrestoEngine PRIVATE tinygltf)

# Needed for shared library
set_property(TARGET PrestoEngine PROPERTY POSIT﻿ION_INDEPENDENT_CODE ON)
set_property(TARGET tinygltf PROPERTY POSITION_INDEPENDENT_CODE ON)

set(PRESTO_BASE_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders)
compileshadersfortarget(PrestoEngine "${PRESTO_BASE_SHADER_DIR}")
