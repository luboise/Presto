cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

find_program(GLSLC_PATH NAMES glslc HINTS Vulkan::glslc)

project(Presto)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "Build Configurations" FORCE)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path" FORCE)
endif()

add_subdirectory(ext/spdlog)
set_property(TARGET spdlog PROPERTY POSITION_INDEPENDENT_CODE ON)

add_subdirectory(ext/GLFW)
link_libraries(glfw)

add_subdirectory(ext/glm)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

macro(CompileShadersForTarget TARGET_NAME SHADER_SOURCE_DIR)
	# set(CUSTOM_TARGET_NAME ${TARGET_NAME}Shaders)
	# add_dependencies(${TARGET_NAME} ${CUSTOM_TARGET_NAME})
	# add_custom_target(${CUSTOM_TARGET_NAME})

	# message("Added custom shader target: " ${CUSTOM_TARGET_NAME})

	message("Beginning shader compilation for target \"" ${TARGET_NAME} "\"")

	# set(WORKING_DIRECTORY ${SHADER_SOURCE_DIR})
	set(SHADER_BUILD_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>/Shaders/)
	file(GLOB_RECURSE SHADER_FILES ${SHADER_SOURCE_DIR}/*)

	# Create a custom target for shader compilation
	foreach(SHADER_FILE ${SHADER_FILES})
		# Get input structure
		string(REPLACE ${SHADER_SOURCE_DIR}/ "" IN_STRUCTURE_FULL ${SHADER_FILE})
		get_filename_component(IN_STRUCTURE ${IN_STRUCTURE_FULL} DIRECTORY)
		
		# Add a slash if the name is not empty
		if(NOT IN_STRUCTURE STREQUAL "")
			set(IN_STRUCTURE ${IN_STRUCTURE}/)
		endif()

		# Get replaced filename
		get_filename_component(FILE_EXTENSION ${SHADER_FILE} EXT)
		string(REPLACE "." "" FILE_EXTENSION ${FILE_EXTENSION})
		get_filename_component(BASE_FILENAME ${SHADER_FILE} NAME_WE)
		set(OUT_FILENAME ${BASE_FILENAME}_${FILE_EXTENSION}.spv)

		# Skip shaders which have already been compiled
		if(FILE_EXTENSION STREQUAL "spv")
			get_filename_component(InputFolderName ${SHADER_SOURCE_DIR} NAME_WE)
			message("-- Skipping \"" ${SHADER_FILE} "\"")
			continue()
		endif()

		message("-- Adding shader \"/" ${IN_STRUCTURE_FULL} "\" -> \"" shaders/${IN_STRUCTURE_DIR}${OUT_FILENAME} "\"")

		set(OUTDIR ${SHADER_BUILD_DIR}${IN_STRUCTURE})
		set(OUTPATH ${OUTDIR}${OUT_FILENAME})

		# Debug messages
		# message("BUILD DIR: " ${SHADER_BUILD_DIR})
		# message("IN STRUCTURE: " ${IN_STRUCTURE})
		# message("OUTDIR: " ${OUTDIR})
		# message("OUTPATH: " ${OUTPATH})

		add_custom_command(
			TARGET ${TARGET_NAME}

			# COMMENT "Compiling  ${SHADER_FILE}  to  ${OUTPATH}"
			COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTDIR}
			COMMAND ${GLSLC_PATH} ${SHADER_FILE} -o ${OUTPATH}

			# COMMENT Built ${OUTPATH}
		)
	endforeach()
endmacro()

if (WIN32)
	set(PLATFORM Windows)

	add_compile_definitions(PR_PLATFORM_WINDOWS)
	add_compile_definitions(NOMINMAX)
	
	# set(CMAKE_GENERATOR_PLATFORM "x64"
	# 	CACHE STRING "Platform (x64)" FORCE
	# )
elseif(UNIX)
	set(PLATFORM Unix)
	add_compile_definitions(PR_PLATFORM_UNIX)
else()
	message(FATAL_ERROR "Platform not selected. Expected Windows or Unix platform for compilation.")
endif()

add_subdirectory(Presto)
target_link_libraries(Presto
	PRIVATE glfw ${Vulkan_LIBRARIES}
	PUBLIC spdlog glm
)
target_include_directories(Presto
	PRIVATE ext/spdlog/include ext/glm/include
)
# target_link_libraries(Presto PRIVATE glfw)
if(PLATFORM STREQUAL "Windows")
	target_link_libraries(Presto PRIVATE dwmapi.lib XInput9_1_0.lib)
endif()


add_subdirectory(Duality)
target_include_directories(Duality PRIVATE ${PRESTO_INCLUDE_DIRS})

# add_subdirectory(PrestoTest)

