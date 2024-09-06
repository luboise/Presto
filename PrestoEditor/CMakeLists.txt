add_executable(PrestoEditor src/main.cpp)

add_dependencies(PrestoEditor Presto)
target_link_libraries(PrestoEditor PRIVATE Presto)

CompileShadersForTarget(PrestoEditor ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)