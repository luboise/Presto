add_subdirectory(Common)
add_subdirectory(Modules)
add_subdirectory(Platform)

target_sources(Presto
	PRIVATE Events/ApplicationEvents.h Events/Event.h Events/KeyEvents.h Events/MouseEvents.h
)

target_include_directories(Presto
	# PRIVATE include
	PRIVATE Events Common Modules Platform
)

# target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(Presto
	PRIVATE Application.cpp Core.h EntryPoint.h Log.h Log.cpp pch.cpp pch.h
	PUBLIC Window.h Application.h
)

add_subdirectory(Components)