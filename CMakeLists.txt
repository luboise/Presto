cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/ext")

find_program(
  GLSLC_PATH
  NAMES glslc
  HINTS Vulkan::glslc)

project(Presto)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../lib)

set(CMAKE_CONFIGURATION_TYPES
    "Debug;Release;Dist"
    CACHE STRING "Build Configurations" FORCE)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      "${CMAKE_BINARY_DIR}/install"
      CACHE PATH "Install path" FORCE)
endif()

if(NOT TARGET spdlog)
  # find_package(spdlog REQUIRED)
  add_subdirectory(ext/spdlog)
endif()
set_property(TARGET spdlog PROPERTY POSITION_INDEPENDENT_CODE ON)

add_subdirectory(ext/tinyobjloader)

set(JSON_BuildTests
    OFF
    CACHE INTERNAL "")
add_subdirectory(ext/json)

# set(glm_DIR ./ext/glm) find_package(glm REQUIRED)

# OpenGL Libraries
find_package(OpenGL REQUIRED)
add_subdirectory(ext/GLFW)
find_package(GLEW REQUIRED)

add_subdirectory(ext/glm)
# find_package(glm REQUIRED PATHS ./ext/glm)

find_package(Vulkan REQUIRED)
# include_directories(${Vulkan_INCLUDE_DIR})

# Macro which compiles shaders in the src/shaders directory to vulkan shader
# units
macro(CompileShadersForTarget target_name shader_source_dir)
  # set(CUSTOM_target_name ${target_name}Shaders)
  # add_dependencies(${target_name} ${CUSTOM_target_name})
  # add_custom_target(${CUSTOM_target_name})

  # message("Added custom shader target: " ${CUSTOM_target_name})

  message("Beginning shader compilation for target \"" ${target_name}
          "\". Reading shaders from " ${shader_source_dir})

  if("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" STREQUAL "")
    message(
      "CMAKE_RUNTIME_OUTPUT_DIRECTORY has not been specified. Shader compilation will be skipped."
    )
  else()

    # set(WORKING_DIRECTORY ${shader_source_dir})
    set(SHADER_BUILD_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Shaders/)
    file(GLOB_RECURSE shader_files "${shader_source_dir}/*")

    if("${shader_files}" STREQUAL "")
      message(
        FATAL_ERROR
          "Base shaders not found. Aborting build."
          "\nShader search location: ${shader_source_dir}"
          "\nTarget: ${target_name}")
    else()

      # Create a custom target for shader compilation
      foreach(shader_file ${shader_files})
        # Get input structure
        string(REPLACE ${shader_source_dir}/ "" IN_STRUCTURE_FULL
                       ${shader_file})
        get_filename_component(IN_STRUCTURE ${IN_STRUCTURE_FULL} DIRECTORY)
        # Add a slash if the name is not empty
        if(NOT IN_STRUCTURE STREQUAL "")
          set(IN_STRUCTURE ${IN_STRUCTURE}/)
        endif()

        if(${IN_STRUCTURE} MATCHES "^OpenGL*")
          string(REPLACE "OpenGL/" "" OUT_FOLDER ${IN_STRUCTURE})

          set(OUTDIR ${SHADER_BUILD_DIR}${OUT_FOLDER})

          set(OUT_FILENAME ${BASE_FILENAME}_${FILE_EXTENSION}.spv)

          get_filename_component(BASE_FILENAME ${shader_file} NAME)
          # set(OUTPATH ${OUTDIR}${BASE_FILENAME})

          message(
            "Copying OpenGL shader ${shader_file} to ${OUTDIR}${BASE_FILENAME}")
          file(COPY ${shader_file} DESTINATION ${OUTDIR})
          continue()

        endif()

        # Get replaced filename
        get_filename_component(FILE_EXTENSION ${shader_file} EXT)
        string(REPLACE "." "" FILE_EXTENSION ${FILE_EXTENSION})
        get_filename_component(BASE_FILENAME ${shader_file} NAME_WE)
        set(OUT_FILENAME ${BASE_FILENAME}_${FILE_EXTENSION}.spv)

        # Skip shaders which have already been compiled
        if(FILE_EXTENSION STREQUAL "spv")
          get_filename_component(InputFolderName ${shader_source_dir} NAME_WE)
          message("-- Skipping \"" ${shader_file} "\"")
          continue()
        endif()

        set(OUTDIR ${SHADER_BUILD_DIR}${IN_STRUCTURE})
        set(OUTPATH ${OUTDIR}${OUT_FILENAME})

        message("-- Adding shader \"/" ${IN_STRUCTURE_FULL} "\" -> \""
                ${OUTPATH}"\"")

        # Debug messages message("BUILD DIR: " ${SHADER_BUILD_DIR}) message("IN
        # STRUCTURE: " ${IN_STRUCTURE}) message("OUTDIR: " ${OUTDIR})
        # message("OUTPATH: " ${OUTPATH})

        add_custom_command(
          TARGET ${target_name}
          COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTDIR}
          COMMAND ${GLSLC_PATH} ${shader_file} -o ${OUTPATH}
          COMMENT "Compiling  ${shader_file}  to  ${OUTPATH}")
      endforeach()
    endif()
  endif()
endmacro()

if(WIN32)
  set(PLATFORM Windows)

  add_compile_definitions(PR_PLATFORM_WINDOWS)
  add_compile_definitions(NOMINMAX)

  set(CMAKE_GENERATOR_PLATFORM
      "x64"
      CACHE STRING "Platform (x64)" FORCE)
elseif(UNIX)
  set(PLATFORM Unix)
  add_compile_definitions(PR_PLATFORM_UNIX)
else()
  message(FATAL_ERROR "Platform not selected."
                      "Expected Windows or Unix platform for compilation.")
endif()

add_library(Presto STATIC)

target_include_directories(Presto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Presto PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(Presto PUBLIC ext/spdlog/include ext/glm/include)
target_include_directories(Presto PRIVATE ${OPENGL_INCLUDE_DIR}
                                          ${GLEW_INCLUDE_DIRS})

target_link_libraries(Presto PRIVATE nlohmann_json::nlohmann_json)

add_subdirectory(src)

# target_link_libraries(Presto PRIVATE glfw)
if(PLATFORM STREQUAL "Windows")
  target_link_libraries(Presto PRIVATE dwmapi.lib XInput9_1_0.lib)
endif()

target_link_libraries(Presto PUBLIC glfw ${Vulkan_LIBRARIES} spdlog glm::glm)
target_link_libraries(Presto PRIVATE ${OPENGL_LIBRARIES} glfw GLEW::GLEW
                                     tinyobjloader)

# Needed for shared library
set_property(TARGET Presto PROPERTY POSITION_INDEPENDENT_CODE ON)

# if (WIN32) target_compile_definitions(Presto PRIVATE PR_BUILD_LIB ) endif()

target_compile_definitions(Presto PRIVATE PR_BUILD_LIB)

target_precompile_headers(Presto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h)

# include(${CMAKE_SOURCE_DIR}/cmake_build_shaders.cmake)

set(PRESTO_BASE_SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders)

compileshadersfortarget(Presto "${PRESTO_BASE_SHADER_DIR}")
