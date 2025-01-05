add_subdirectory(DrawManager)
add_subdirectory(PipelineManager)
add_subdirectory(DescriptorManager)

target_sources(PrestoEngine PRIVATE DescriptorManager/DescriptorManager.cpp
                                    BufferManager/BufferManager.cpp)
