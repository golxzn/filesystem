
if(NOT GXZN_OS_FS_DEV_MODE)
	return()
endif()

if(GXZN_OS_FS_BUILD_TEST)
	enable_testing()
	add_subdirectory(${GXZN_OS_FS_TEST_DIR}) # filesystem tests (only if project is not included as subproject)
endif()

if(GXZN_OS_FS_GENERATE_DOCS)
	include(${GXZN_OS_FS_ROOT}/cmake/automatics/docs.cmake)
endif()
