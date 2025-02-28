target_precompile_headers(${PRESTO_ENGINE_NAME} PRIVATE
                          ${CMAKE_CURRENT_SOURCE_DIR}/pch.h)

add_subdirectory(API)

add_subdirectory(Assets)
add_subdirectory(Common)
add_subdirectory(Components)
add_subdirectory(Modules)
add_subdirectory(Objects)
add_subdirectory(Physics)
add_subdirectory(Rendering)
add_subdirectory(Shaders)
add_subdirectory(Utils)

if(PRESTO_DEBUG_BUILD)
  add_subdirectory(Debugging)
endif()

target_sources(
  ${PRESTO_ENGINE_NAME}
  PRIVATE Application.cpp
          # Rendering/RenderLayer.cpp
          Rendering/RenderingManager.cpp
          Runtime/Time.cpp
          Materials.cpp
          Logging.cpp
          pch.cpp
          GLFWAppWindow.cpp)

# add_subdirectory(Platform)
