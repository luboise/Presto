target_sources(Presto
# PUBLIC	Allocator.h
PUBLIC	Camera.h

PRIVATE	Camera.cpp
)


target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})