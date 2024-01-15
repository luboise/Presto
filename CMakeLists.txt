cmake_minimum_required(VERSION 3.28)
project(Presto)

IF (WIN32)
	add_compile_definitions(PR_PLATFORM_WINDOWS)
ENDIF()

add_subdirectory(external/spdlog)
target_include_directories(spdlog INTERFACE ${spdlog_SOURCE_DIR}/include)

add_subdirectory(external/GLFW)
link_libraries(GLFW)

link_directories(libs)

add_subdirectory(Presto)
target_link_libraries(Presto PUBLIC spdlog)