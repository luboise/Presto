if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${PLATFORM})
	add_subdirectory(${PLATFORM})
else()
	message(FATAL_ERROR "Platform files do not exist for platform \"" ${PLATFORM}
																						"\"")
endif()
