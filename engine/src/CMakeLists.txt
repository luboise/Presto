add_subdirectory(impl)

# add_subdirectory(assets)

if(PRESTO_DEBUG_BUILD)
  internal_modules(debugging.cppm)
  internal_header_modules(debuggingconstants.cppm)

  if(NOT DEFINED IMGUI_DIR)
    message(
      FATAL_ERROR
      "IMGUI_DIR is not defined. The project can not link the debug libraries.")
  endif()

  target_include_directories(${PRESTO_ENGINE_NAME} PRIVATE ${IMGUI_DIR})

  target_sources(
    ${PRESTO_ENGINE_NAME}
    PRIVATE ${IMGUI_DIR}/imgui.cpp
          ${IMGUI_DIR}/imgui_draw.cpp
          ${IMGUI_DIR}/imgui_tables.cpp
          ${IMGUI_DIR}/imgui_widgets.cpp
          ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
          ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
          ${IMGUI_DIR}/imgui_demo.cpp)
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

internal_modules(
	managers/asset.cppm
	managers/physics.cppm
	managers/rendering.cppm
	managers/entityimpl.cppm
    managers.cppm
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
	rendering/buffer.cppm
	rendering/shader.cppm
	rendering/pipeline.cppm
	rendering/opengl/buffer.cppm
	rendering/opengl/opengl.cppm
	rendering/opengl/pipeline.cppm
	rendering/opengl/shader.cppm
	rendering/opengl/texture.cppm
)

add_subdirectory(resources)
