cmake_minimum_required(VERSION 3.23)

project(resman
	VERSION 1.1.1
	DESCRIPTION "golxzn resource manager"
	LANGUAGES CXX
)

include(cmake/options.cmake)
include(cmake/definitions.cmake)

add_subdirectory(${GRM_CODE_DIR}) # golxzn::resman target
add_subdirectory(${GRM_TEST_DIR}) # resman tests (only if project is not included as subproject)
