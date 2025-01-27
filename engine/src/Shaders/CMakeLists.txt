# Shader compilation details below

# Macro which compiles shaders in the src/shaders directory to vulkan shader
# units
macro(compileshadersfortarget target_name shader_source_dir)
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
        get_filename_component(FILE_EXTENSION ${shader_file} EXT)

        if("${FILE_EXTENSION}" STREQUAL ".txt")
          message("-- Skipping text file \"" ${shader_file} "\"")
          continue()
        endif()

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
          POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTDIR}
          COMMAND ${GLSLC_PATH} ${shader_file} -o ${OUTPATH}
          COMMENT "Compiling  ${shader_file}  to  ${OUTPATH}")
      endforeach()
    endif()
  endif()
endmacro()
