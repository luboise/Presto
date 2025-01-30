project(Presto)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

# find_package(glm REQUIRED PATHS ./ext/glm)

# Engine definitions
add_library(${PRESTO_ENGINE_NAME} SHARED)

target_include_directories(${PRESTO_ENGINE_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(
  ${PRESTO_ENGINE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ext/spdlog/include
                               ${CMAKE_CURRENT_SOURCE_DIR}/../ext/glm/include)

target_include_directories(
  ${PRESTO_ENGINE_NAME}
  PRIVATE ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS}
          ${CMAKE_CURRENT_SOURCE_DIR}/src
          ${CMAKE_CURRENT_SOURCE_DIR}/../ext/stb/)

target_link_libraries(${PRESTO_ENGINE_NAME} PUBLIC nlohmann_json::nlohmann_json
                                                   glm::glm)

# target_link_libraries(${PRESTO_ENGINE_NAME} PRIVATE glfw)
if(PLATFORM STREQUAL "Windows")
  target_link_libraries(${PRESTO_ENGINE_NAME} PRIVATE dwmapi.lib
                                                      XInput9_1_0.lib)
endif()

# Needed for shared library
set_property(TARGET ${PRESTO_ENGINE_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(${PRESTO_ENGINE_NAME} PRIVATE PR_BUILD_LIB)
target_compile_options(${PRESTO_ENGINE_NAME} PRIVATE -Wall -Wextra)

add_subdirectory(src)

# target_link_libraries(${PRESTO_ENGINE_NAME} PRIVATE glfw)
if(PLATFORM STREQUAL "Windows")
  target_link_libraries(${PRESTO_ENGINE_NAME} PRIVATE dwmapi.lib
                                                      XInput9_1_0.lib)
endif()

target_link_libraries(${PRESTO_ENGINE_NAME} PUBLIC glfw ${Vulkan_LIBRARIES}
                                                   spdlog)

set(TINYGLTF_HEADER_ONLY
    OFF
    CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL
    OFF
    CACHE INTERNAL "" FORCE)

set(PRESTO_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

# target_include_directories(${PRESTO_ENGINE_NAME} PUBLIC
# ${CMAKE_CURRENT_SOURCE_DIR}/ext/TinyGLTF)

target_link_libraries(${PRESTO_ENGINE_NAME} PRIVATE ${OPENGL_LIBRARIES} glfw
                                                    GLEW::GLEW)
target_link_libraries(${PRESTO_ENGINE_NAME} PRIVATE tinygltf)

# Needed for shared library
set_property(TARGET ${PRESTO_ENGINE_NAME} PROPERTY POSIT﻿ION_INDEPENDENT_CODE
                                                   ON)
set_property(TARGET tinygltf PROPERTY POSITION_INDEPENDENT_CODE ON)

set(PRESTO_BASE_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders)
compileshadersfortarget(${PRESTO_ENGINE_NAME} "${PRESTO_BASE_SHADER_DIR}")
