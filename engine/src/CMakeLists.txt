add_subdirectory(impl)

# add_subdirectory(assets)

if(PRESTO_DEBUG_BUILD)
  internal_modules(debuggingconstants.cppm debugging.cppm)

  if(NOT DEFINED IMGUI_DIR)
    message(
      FATAL_ERROR
        "IMGUI_DIR is not defined. The project can not link the debug libraries."
    )
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
  #
  windowdata.cppm)

target_sources(${PRESTO_ENGINE_NAME} 	PRIVATE
../src/impl/core/logging.cpp
../src/impl/core/eventmanager.cpp
#
../src/impl/assets/types.cpp
../src/impl/assets.cpp
../src/impl/assets/font.cpp
../src/impl/assets/image.cpp
../src/impl/assets/mesh.cpp
../src/impl/assets/model.cpp
../src/impl/assets/material.cpp
#
# ../src/impl/math.cpp
../src/impl/math/shapes.cpp
#
../src/impl/objects.cpp
../src/impl/objects/entity.cpp
../src/impl/objects/figure.cpp
../src/impl/objects/components/camera.cpp
../src/impl/objects/components/canvas.cpp
../src/impl/objects/components/canvasitems.cpp
../src/impl/objects/components/canvastypes.cpp
../src/impl/objects/components/conductor.cpp
../src/impl/objects/components/render.cpp
../src/impl/objects/components/rendersubcomps.cpp
../src/impl/objects/components/rigidbody.cpp
../src/impl/objects/components/transform.cpp
#
../src/impl/runtime.cpp
../src/impl/runtime/application.cpp
../src/impl/runtime/time.cpp
#
../src/impl/utils/file.cpp
#
../src/impl/materials.cpp
../src/impl/system.cpp)

add_subdirectory(resources)
