add_subdirectory(Common)
add_subdirectory(Platform)
add_subdirectory(Components)
add_subdirectory(Objects)
add_subdirectory(Rendering)

target_sources(Presto PRIVATE Application.cpp Log.cpp pch.cpp GLFWAppWindow.cpp)
target_sources(Presto PRIVATE ResourcesModule/ResourceManager.cpp)
