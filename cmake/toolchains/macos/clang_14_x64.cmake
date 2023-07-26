
set(CMAKE_SYSTEM_NAME Darwin CACHE STRING "System name")
set(CMAKE_SYSTEM_PROCESSOR x86_64 CACHE STRING "System processor")
set(CMAKE_HOST_SYSTEM_PROCESSOR x86_64 CACHE STRING "Host system processor")

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER_VERSION 14)
set(CMAKE_CXX_COMPILER_VERSION 14)

find_program(ccache_program ccache)
if (NOT ${ccache_program} MATCHES "NOTFOUND")
	set(CMAKE_C_COMPILER_LAUNCHER ccache)
	set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif()

