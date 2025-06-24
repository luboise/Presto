target_include_directories(
  ${PRESTO_ENGINE_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/internal_headers")

# add_subdirectory(assets)

if(PRESTO_DEBUG_BUILD)
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

target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE core/logging.cpp
          core/event_manager.cpp
          #
          assets/asset.cpp
          assets.cpp
          assets/font.cpp
          assets/image.cpp
          assets/mesh.cpp
          assets/model.cpp
          assets/material.cpp
          #
          # math.cpp
          math/shapes.cpp
          #
          physics.cpp
          #
          objects.cpp
          objects/entity.cpp
          objects/figure.cpp
          objects/components/camera.cpp
          objects/components/canvas.cpp
          objects/components/canvasitems.cpp
          objects/components/canvastypes.cpp
          objects/components/conductor.cpp
          objects/components/render.cpp
          objects/components/rendersubcomps.cpp
          objects/components/rigidbody.cpp
          objects/components/transform.cpp
          #
          runtime.cpp
          runtime/application.cpp
          runtime/time.cpp
          #
          utils/file.cpp
          #
          materials.cpp
          system.cpp)

add_subdirectory(resources)
