
macro(set_found_program_if_exitst varname program hints)
	find_program(${program}_found ${program} HINTS ${hints})
	if (NOT ${${program}_found} MATCHES "NOTFOUND")
		message(VERBOSE "Program ${program} found at ${${program}_found}")
		set(${varname} ${${program}_found})
	else()
		message(VERBOSE "Program ${program} not found")
	endif()
endmacro()

set(COMMON_PATH ${CMAKE_CURRENT_LIST_DIR}/common CACHE PATH "")
list(APPEND CMAKE_MODULE_PATH ${COMMON_PATH})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

include(SetupWindowsEnv)
setup_windows_environment(x64)

set(cl_path ${CMAKE_VS_ROOT}/VC/Tools/MSVC/${CMAKE_MSVC_VERSION}/bin/HostX64/x64)
set(rc_path ${CMAKE_KITS_DIRECTORY}/bin/${CMAKE_SYSTEM_VERSION}/x64)

set_found_program_if_exitst(CMAKE_CXX_COMPILER cl ${cl_path})
set_found_program_if_exitst(CMAKE_C_COMPILER cl ${cl_path})
set_found_program_if_exitst(CMAKE_RC_COMPILER rc ${rc_path})
set_found_program_if_exitst(CMAKE_MT mt ${rc_path})

find_program(ccache_program ccache)
if (NOT ${ccache_program} MATCHES "NOTFOUND")
	set(CMAKE_C_COMPILER_LAUNCHER ccache)
	set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif()

