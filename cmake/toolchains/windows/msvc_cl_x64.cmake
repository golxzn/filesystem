
set(COMMON_PATH ${CMAKE_CURRENT_LIST_DIR}/common CACHE PATH "")
list(APPEND CMAKE_MODULE_PATH ${COMMON_PATH})


set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

include(SetupWindowsEnv)
setup_windows_environment(x64)

set(cl_path ${CMAKE_VS_ROOT}/VC/Tools/MSVC/${CMAKE_MSVC_VERSION}/bin/HostX64/x64)
set(rc_path ${CMAKE_KITS_DIRECTORY}/bin/${CMAKE_SYSTEM_VERSION}/x64)

find_program(CMAKE_CXX_COMPILER cl HINTS ${cl_path})
find_program(CMAKE_C_COMPILER cl HINTS ${cl_path})
find_program(CMAKE_RC_COMPILER rc HINTS ${rc_path})
find_program(CMAKE_MT mt HINTS ${rc_path})

find_program(ccache_program ccache)
if (NOT ${ccache_program} MATCHES "NOTFOUND")
	set(CMAKE_C_COMPILER_LAUNCHER ccache)
	set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif()


message(VERBOSE "CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message(VERBOSE "CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message(VERBOSE "CMAKE_RC_COMPILER: ${CMAKE_RC_COMPILER}")
message(VERBOSE "CMAKE_MT_COMPILER: ${CMAKE_MT_COMPILER}")

