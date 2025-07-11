add_subdirectory(impl)
target_include_directories(${PRESTO_ENGINE_NAME}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

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

target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE FILE_SET
          presto_internal_modules
          TYPE
          CXX_MODULES
          FILES
          loaders/loader_interfaces.cppm
          loaders/image_loader.cppm
          loaders/gltf_loader.cppm
          loading.cppm
          #
          common.cppm
          common.types.cppm
          events.cppm
          #
          create_window.cppm
          #
          defaults.cppm
          defaults/meshes.cppm
          defaults/textures.cppm
          #
          glfw.cppm
          glfw-base.cppm
          glfw-impl.cppm
          #
          rendering/types.cppm
          rendering/pipeline.cppm
          rendering/renderer.cppm
          rendering/vertex.cppm
          rendering/shader.cppm
          rendering.cppm
          #
          rendering/opengl/types.cppm
          rendering/opengl/buffer.cppm
          rendering/opengl/pipeline.cppm
          rendering/opengl/shader.cppm
          rendering/opengl/texture.cppm
          rendering/opengl/opengl.cppm
          #
          managers/manager.cppm
          managers/debug_manager.cppm
          managers/asset_manager.cppm
          managers/entity_manager_impl.cppm
          managers/event_manager_impl.cppm
          managers/physics_manager.cppm
          managers/rendering_manager.cppm
          managers.cppm
          #
          window_data.cppm
          #
          struct_uniform_buffer.cppm
          internal.cppm
  PRIVATE managers/asset_manager.cpp
          managers/entity_manager_impl.cpp
          managers/event_manager_impl.cpp
          managers/physics_manager.cpp
          managers/rendering_manager.cpp
          #
          impl/utils.cpp
          #
          impl/core/logging.cpp
          impl/core/event_manager.cpp
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
          impl/math/quaternion.cpp
          impl/math/transform_data.cpp
          #
          impl/physics.cpp
          #
          impl/objects/base.cpp
          impl/objects/figure.cpp
          impl/objects/components/camera.cpp
          impl/objects/components/canvas.cpp
          impl/objects/components/canvas_items.cpp
          impl/objects/components/canvas_types.cpp
          impl/objects/components/render.cpp
          impl/objects/components/rendersubcomps.cpp
          impl/objects/components/rigidbody.cpp
          impl/objects/components/transform.cpp
          impl/objects.cpp
          #
          impl/runtime.cpp
          impl/runtime/application.cpp
          impl/runtime/time.cpp
          #
          impl/utils/file.cpp
          #
          impl/system.cpp
          #
          create_window.cpp)

add_subdirectory(resources)
