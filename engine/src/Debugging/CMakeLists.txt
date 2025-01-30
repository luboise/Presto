# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

# Engine definitions
target_sources(${PRESTO_ENGINE_NAME} PRIVATE DebugUI.cpp)

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
