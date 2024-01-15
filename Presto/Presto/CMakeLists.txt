target_precompile_headers(Presto PUBLIC prpch.h)

add_subdirectory(Common)
add_subdirectory(Modules)
add_subdirectory(Platform)

target_include_directories(Presto PUBLIC include PRIVATE Events PRIVATE Common PRIVATE Modules PRIVATE Objects PRIVATE Platform)

target_sources(Presto PUBLIC Application.cpp InputManager.cpp Log.cpp prpch.cpp)

target_include_directories(Presto PUBLIC include)
# target_include_header(Presto PRIVATE prpch.h)