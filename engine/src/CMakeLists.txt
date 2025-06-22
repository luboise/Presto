add_subdirectory(impl)
target_include_directories(${PRESTO_ENGINE_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

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

target_sources(${PRESTO_ENGINE_NAME} PRIVATE FILE_SET presto_internal_modules
                                               TYPE CXX_MODULES FILES 
  common.cppm
  common.types.cppm
  #
  defaults.cppm
  defaults/meshes.cppm
  defaults/textures.cppm
  #
  rendering/types.cppm
  rendering/vertex.cppm
  rendering/base.cppm
  rendering/buffer.cppm
  rendering/shader.cppm
  rendering/pipeline.cppm
  rendering.cppm
  rendering/opengl/buffer.cppm
  rendering/opengl/opengl.cppm
  rendering/opengl/pipeline.cppm
  rendering/opengl/shader.cppm
  rendering/opengl/texture.cppm
  #
  managers/asset.cppm
  managers/physics.cppm
  managers/rendering.cppm
  managers/entityimpl.cppm
  managers.cppm
  #
  windowdata.cppm)

target_sources(${PRESTO_ENGINE_NAME} 	PRIVATE
impl/core/logging.cpp
impl/core/eventmanager.cpp
#
impl/assets/types.cpp
impl/assets.cpp
impl/assets/font.cpp
impl/assets/image.cpp
impl/assets/mesh.cpp
impl/assets/model.cpp
impl/assets/material.cpp
#
# impl/math.cpp
impl/math/shapes.cpp
#
impl/objects.cpp
impl/objects/entity.cpp
impl/objects/figure.cpp
impl/objects/components/camera.cpp
impl/objects/components/canvas.cpp
impl/objects/components/canvasitems.cpp
impl/objects/components/canvastypes.cpp
impl/objects/components/conductor.cpp
impl/objects/components/render.cpp
impl/objects/components/rendersubcomps.cpp
impl/objects/components/rigidbody.cpp
impl/objects/components/transform.cpp
#
impl/runtime.cpp
impl/runtime/application.cpp
impl/runtime/time.cpp
#
impl/utils/file.cpp
#
impl/materials.cpp
impl/system.cpp)

add_subdirectory(resources)
