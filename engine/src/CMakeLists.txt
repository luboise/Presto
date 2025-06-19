add_subdirectory(impl)

# add_subdirectory(assets)
add_subdirectory(Common)
add_subdirectory(Components)
add_subdirectory(Modules)
add_subdirectory(Physics)
add_subdirectory(Rendering)
add_subdirectory(Runtime)
add_subdirectory(Utils)

if(PRESTO_DEBUG_BUILD)
  add_subdirectory(Debugging)
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

internal_modules(
    managers.cppm
	managers/asset.cppm
	managers/physics.cppm
	managers/rendering.cppm
	managers/entityimpl.cppm
	#
    pch.cppm
	#
    common.cppm
    common.types.cppm
	#
    defaults.cppm
    defaults.meshes.cppm
    defaults.textures.cppm
	#
	rendering/base.cppm
	rendering/types.cppm
	rendering/opengl/buffer.cppm
	rendering/opengl/opengl.cppm
	rendering/opengl/pipeline.cppm
	rendering/opengl/shader.cppm
	rendering/opengl/texture.cppm
)

# internal_modules(core.logging.cpp)

add_subdirectory(cppmodules)

# add_subdirectory(Platform)
