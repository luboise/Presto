set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

internal_modules(
    managers.cppm
    pch.cppm
    common.cppm
    common.types.cppm
    defaults.cppm
    defaults.meshes.cppm
    defaults.textures.cppm)
