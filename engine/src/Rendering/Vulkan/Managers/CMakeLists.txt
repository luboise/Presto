add_subdirectory(DrawManager)
add_subdirectory(PipelineManager)
add_subdirectory(DescriptorManager)

target_sources(${PRESTO_ENGINE_NAME} PRIVATE DescriptorManager/DescriptorManager.cpp
                                    BufferManager/BufferManager.cpp)
