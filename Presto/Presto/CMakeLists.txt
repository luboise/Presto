add_subdirectory(Common)
add_subdirectory(Modules)
add_subdirectory(Platform)
add_subdirectory(Objects)
target_sources(Presto PRIVATE Objects/Object.h)

target_sources(Presto
	PRIVATE Events/ApplicationEvents.h Events/Event.h Events/KeyEvents.h Events/MouseEvents.h
)

target_include_directories(Presto
	# PRIVATE include
	PRIVATE Events Common Modules Objects Platform
)

# target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(Presto
	PRIVATE Application.cpp Application.h Core.h EntryPoint.h InputManager.cpp InputManager.h Log.h Log.cpp prpch.cpp prpch.h Window.h
)