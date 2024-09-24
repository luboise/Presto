add_subdirectory(DrawManager)
add_subdirectory(PipelineManager)
add_subdirectory(DescriptorManager)

target_sources(Presto PRIVATE
        DescriptorManager/DescriptorManager.cpp
        BufferManager/BufferManager.cpp
)