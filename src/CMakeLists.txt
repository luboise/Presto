add_subdirectory(Common)
add_subdirectory(Components)
add_subdirectory(Objects)
add_subdirectory(Rendering)
add_subdirectory(Utils)
add_subdirectory(Scenes)

# add_subdirectory(Platform)

target_sources(Presto PRIVATE Application.cpp Log.cpp pch.cpp GLFWAppWindow.cpp)
target_sources(Presto PRIVATE ResourcesModule/ResourceManager.cpp)
