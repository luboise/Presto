add_library(Presto SHARED)

target_include_directories(Presto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(Presto)
