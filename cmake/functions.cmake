function(api_modules files)
		# target_sources(${PRESTO_ENGINE_NAME} PUBLIC
		# FILE_SET presto_modules TYPE CXX_MODULES FILES ${files})
	foreach(file ${files})
		# /home/lucas/dev/personal/Presto/engine/modules/x -> 
		# /home/lucas/dev/personal/Presto/engine/src/x


		# Rename cppm to cpp

		file(RELATIVE_PATH rel_path
			"${PROJECT_SOURCE_DIR}/engine/modules"
			"${CMAKE_CURRENT_SOURCE_DIR}/${file}")
		message(STATUS ${rel_path})

		string(REPLACE ".cppm" ".cpp" impl_path "${rel_path}")

		target_sources(${PRESTO_ENGINE_NAME}
			PUBLIC
				FILE_SET presto_modules TYPE CXX_MODULES
				FILES "${CMAKE_CURRENT_SOURCE_DIR}/${file}"
			PRIVATE
				 "${PROJECT_SOURCE_DIR}/engine/src/impl/${impl_path}")
	endforeach()
endfunction()

function(api_header_modules files)
	foreach(file ${files})
		target_sources(${PRESTO_ENGINE_NAME}
			PUBLIC
				FILE_SET presto_modules TYPE CXX_MODULES
				FILES "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
	endforeach()
endfunction()



# function(implementation_modules files)
# 		# target_sources(${PRESTO_ENGINE_NAME} PUBLIC
# 		# FILE_SET presto_modules TYPE CXX_MODULES FILES ${files})
# 	foreach(file ${files})
# 		target_sources(${PRESTO_ENGINE_NAME} PRIVATE
# 			FILE_SET presto_modules TYPE CXX_MODULES FILES ${CMAKE_CURRENT_SOURCE_DIR}/${file})
# 	   endforeach()
# endfunction()

function(internal_header_modules files)
	foreach(file ${files})
		target_sources(${PRESTO_ENGINE_NAME}
			 PRIVATE
			 	FILE_SET presto_internal_modules TYPE CXX_MODULES
				FILES "${CMAKE_CURRENT_SOURCE_DIR}/${file}")
    endforeach()
endfunction()

function(internal_modules files)
	foreach(file ${files})
		string(REPLACE ".cppm" ".cpp" source_path "${CMAKE_CURRENT_SOURCE_DIR}/${file}")

		target_sources(${PRESTO_ENGINE_NAME}
			 PRIVATE
			 	FILE_SET presto_internal_modules TYPE CXX_MODULES
				FILES "${CMAKE_CURRENT_SOURCE_DIR}/${file}"

		 	 PRIVATE
			 	"${source_path}")
    endforeach()
endfunction()
