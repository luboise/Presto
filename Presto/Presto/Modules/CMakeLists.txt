target_sources(Presto
PUBLIC	Module.h
		InputModule/InputManager.h
		ResourcesModule/ResourceManager.h
		_ModulesHeader.h

PRIVATE	ResourcesModule/ResourceManager.cpp


)

target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(RenderingModule)
add_subdirectory(ObjectsModule)