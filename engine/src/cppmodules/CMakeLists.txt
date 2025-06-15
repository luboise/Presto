target_sources(${PRESTO_ENGINE_NAME}
	PRIVATE
	FILE_SET presto_internal_modules TYPE CXX_MODULES FILES
	common.cppm
	common.math.cppm
)

