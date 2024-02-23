target_sources(Presto
PUBLIC	Module.h

PUBLIC	ResourcesModule/ResourceManager.h
PRIVATE	ResourcesModule/ResourceManager.cpp

PUBLIC 	InputModule/InputManager.h
)

target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(RenderingModule)
add_subdirectory(ObjectsModule)