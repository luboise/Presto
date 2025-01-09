project(Presto)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

# Engine definitions
add_executable(PrestoEditor src/main.cpp src/EditorUI.cpp)
target_link_libraries(PrestoEditor PRIVATE PrestoEngine glm::glm)

target_sources(PrestoEditor PRIVATE ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp ${IMGUI_DIR}/imgui_demo.cpp)

target_include_directories(PrestoEditor
	PRIVATE ${PRESTO_INCLUDE_DIRS} ${IMGUI_DIR})
