function(api_modules files)
	foreach(file ${files})
		target_sources(${PRESTO_ENGINE_NAME} PUBLIC
		FILE_SET presto_modules TYPE CXX_MODULES FILES ${file})
    endforeach()
endfunction()

function(internal_modules files)
	foreach(file ${files})
		target_sources(${PRESTO_ENGINE_NAME} PRIVATE
		FILE_SET presto_internal_modules TYPE CXX_MODULES FILES ${file})
    endforeach()
endfunction()
