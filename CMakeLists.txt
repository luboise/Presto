cmake_minimum_required(VERSION 3.15)

project(FirstGameEngine,
	VERSION 1.0
	DESCRIPTION "First game engine ig"
	LANGUAGES CXX)

add_executable(engine
	src/platform/win32_platform.cpp
	src/platform/WindowsInputPoller.cpp
	src/platform/WindowsInputPoller.cpp)

target_compile_features(engine PUBLIC cxx_std_23)