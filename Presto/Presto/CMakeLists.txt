target_precompile_headers(Presto PUBLIC prpch.h)

add_subdirectory(Common)
add_subdirectory(Modules)
add_subdirectory(Platform)

target_include_directories(Presto
	PUBLIC include
	PRIVATE Events Common Modules Objects Platform
)

target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(Presto
	PUBLIC Application.cpp InputManager.h InputManager.cpp Log.cpp prpch.h prpch.cpp Core.h
)
# target_include_header(Presto PRIVATE prpch.h)