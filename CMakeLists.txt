cmake_minimum_required(VERSION 3.15)

project(FirstGameEngine
	VERSION 1.0
	DESCRIPTION "First game engine ig"
	LANGUAGES CXX)



set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
    src/platform/win32_platform.cpp
    src/platform/WindowsInputPoller.cpp
    src/platform/WindowsInputPoller.h
)
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)

# Set the entry point
set_target_properties(${PROJECT_NAME} PROPERTIES
    WIN32_EXECUTABLE true
    LINK_FLAGS "/ENTRY:wWinMainCRTStartup"
)