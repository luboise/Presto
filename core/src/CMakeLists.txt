add_subdirectory(Common)
add_subdirectory(Rendering)

# add_subdirectory(Platform)

target_sources(PrestoCore PRIVATE Logging.cpp pch.cpp GLFWAppWindow.cpp)

add_subdirectory(Modules)
