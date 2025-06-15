set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

api_modules(
	presto.cppm
	#
	core.cppm
	core.types.cppm
	core.constants.cppm
	core.assert.cppm
	#
	assets.cppm
)

add_subdirectory(assets)
