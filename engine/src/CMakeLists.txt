add_subdirectory(impl)
target_include_directories(${PRESTO_ENGINE_NAME}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# add_subdirectory(assets)

if(PRESTO_DEBUG_BUILD)
  add_subdirectory(debugging)
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE impl/core/logging.cpp
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
          impl/physics.cpp
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
