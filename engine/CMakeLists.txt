add_library(PrestoEngine STATIC)

target_include_directories(PrestoEngine
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(PrestoEngine PRIVATE PrestoCore)

# target_link_libraries(PrestoEngine PRIVATE glfw)
if(PLATFORM STREQUAL "Windows")
  target_link_libraries(PrestoEngine PRIVATE dwmapi.lib XInput9_1_0.lib)
endif()

# Needed for shared library set_property(TARGET PrestoEngine PROPERTY
# POSITION_INDEPENDENT_CODE ON)

target_compile_definitions(PrestoEngine PRIVATE PR_BUILD_LIB)

target_precompile_headers(PrestoEngine PUBLIC
                          ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

add_subdirectory(src)

# include(${CMAKE_SOURCE_DIR}/cmake_build_shaders.cmake)

# set(PRESTO_BASE_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders)
# compileshadersfortarget(PrestoEngine "${PRESTO_BASE_SHADER_DIR}")
