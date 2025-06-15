set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

target_sources(${PRESTO_ENGINE_NAME}
  PRIVATE
    FILE_SET internal_modules TYPE CXX_MODULES FILES
	presto.cppm
	defaults.cppm
	defaults.meshes.cppm
	defaults.textures.cppm
)
