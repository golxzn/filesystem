cmake_minimum_required(VERSION 3.23)

set(GRM_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

project(resman
	VERSION 1.2.1
	DESCRIPTION "golxzn resource manager"
	LANGUAGES CXX
)

include(${GRM_ROOT}/cmake/options.cmake)
include(${GRM_ROOT}/cmake/definitions.cmake)

add_subdirectory(${GRM_CODE_DIR}) # golxzn::resman target
add_subdirectory(${GRM_TEST_DIR}) # resman tests (only if project is not included as subproject)
