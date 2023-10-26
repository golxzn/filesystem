cmake_minimum_required(VERSION 3.23)

set(GXZN_OS_FS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

project(golxzn.os.filesystem
	VERSION 1.6.5
	DESCRIPTION "golxzn filesystem submodule"
	LANGUAGES CXX
)

include(${GXZN_OS_FS_ROOT}/cmake/options.cmake)
include(${GXZN_OS_FS_ROOT}/cmake/definitions.cmake)

add_subdirectory(${GXZN_OS_FS_CODE_DIR})

include(${GXZN_OS_FS_ROOT}/cmake/dev-mode.cmake)
