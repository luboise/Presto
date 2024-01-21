add_executable(Duality src/DualityApp.cpp)

add_dependencies(Duality Presto)
target_link_libraries(Duality PRIVATE Presto)

CompileShadersForTarget(Duality ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders)