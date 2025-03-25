# Shader compilation details below

# Macro which compiles shaders in the src/shaders directory to vulkan shader
# units
macro(compile_shaders_for_target target_name IN_SHADER_DIR OUT_SHADER_SUFFIX)

  # set(CUSTOM_target_name ${target_name}Shaders)
  # add_dependencies(${target_name} ${CUSTOM_target_name})
  # add_custom_target(${CUSTOM_target_name})

  # message("Added custom shader target: " ${CUSTOM_target_name})

  message(STATUS "Beginning shader compilation for ${target_name}")
  message(STATUS "Input dir - ${IN_SHADER_DIR}")
  list(APPEND CMAKE_MESSAGE_INDENT "  ")

  unset(PRESTO_SHADERS_COMPILED_SUCCESSFULLY)

  foreach(ignore RANGE 1)
    message(CHECK_START "Check output dir")
    if("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" STREQUAL "")
      message(
        CHECK_FAIL
        "CMAKE_RUNTIME_OUTPUT_DIRECTORY has not been specified. Skipping shader compilation."
      )
      # Return early
      continue()
    endif()

    # set(WORKING_DIRECTORY ${IN_SHADER_DIR})
    set(OUT_SHADER_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${OUT_SHADER_SUFFIX}")

    message(CHECK_PASS "${OUT_SHADER_DIR}")

    message(CHECK_START "Finding shaders")
    file(
      GLOB_RECURSE SHADER_FILES
      LIST_DIRECTORIES false
      RELATIVE "${IN_SHADER_DIR}"
      "${IN_SHADER_DIR}/*")

    list(LENGTH SHADER_FILES NUM_SHADERS)

    if(${NUM_SHADERS} EQUAL 0)
      message(CHECK_FAIL "0 found")
      message(STATUS "Unable to compile shaders.")
      # Return early
      continue()
    endif()

    message(CHECK_PASS "${NUM_SHADERS} found")

    # Create a custom target for shader compilation
    foreach(SHADER_FILE ${SHADER_FILES})
      get_filename_component(FILE_EXTENSION ${SHADER_FILE} EXT)

      # Check file type
      if("${FILE_EXTENSION}" STREQUAL ".txt")
        message(STATUS "\*${FILE_EXTENSION} - Skipping ${SHADER_FILE}")
        # Skip file
        continue()
      endif()

      # If OpenGL, just copy the shader
      if(${SHADER_FILE} MATCHES "^[Oo]pen[Gg][Ll]*")
        set(OUT_FILE "${OUT_SHADER_DIR}/${SHADER_FILE}")

        get_filename_component(OUT_SHADER_DIR_PARENT ${OUT_FILE} DIRECTORY)
        file(COPY "${IN_SHADER_DIR}/${SHADER_FILE}"
             DESTINATION "${OUT_SHADER_DIR_PARENT}")
        continue()
      endif()

      # ########################################################################
      # Vulkan

      # Replace the file extension with spv
      set(OUT_FILE "${OUT_SHADER_DIR}/${SHADER_FILE}")
      string(REPLACE "${EXT}" "${EXT}.spv" OUT_FILE ${OUT_FILE})

      # # Skip shaders which have already been compiled if(FILE_EXTENSION
      # STREQUAL "spv") get_filename_component(InputFolderName ${IN_SHADER_DIR}
      # NAME_WE) message("-- Skipping \"" ${SHADER_FILE} "\"") continue()
      # endif()

      message(STATUS "Compiling vulkan shader ${SHADER_FILE} -> ${OUT_FILE}")

      get_filename_component(OUT_SHADER_DIR_PARENT ${OUT_FILE} DIRECTORY)

      add_custom_command(
        TARGET ${target_name}
        PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUT_SHADER_DIR_PARENT}
        COMMAND ${GLSLC_PATH} "${IN_SHADER_DIR}/${SHADER_FILE}" -o ${OUT_FILE}
        COMMENT "Compiling  ${SHADER_FILE}  to  ${OUT_FILE}")
    endforeach()

    set(PRESTO_SHADERS_COMPILED_SUCCESSFULLY true)
  endforeach()

  list(POP_BACK CMAKE_MESSAGE_INDENT)
endmacro()
