
set(COMMON_PATH ${CMAKE_CURRENT_LIST_DIR}/common CACHE PATH "")
list(APPEND CMAKE_MODULE_PATH ${COMMON_PATH})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)

include(SetupWindowsEnv)
setup_windows_environment(x64)

macro(find_program_default name program)
	if(NOT DEFINED ${name})
		find_program(${name} ${program})

		if(NOT ${name})
			set(${name} ${program} CACHE)
		endif()
	endif()
endmacro()

find_program_default(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_ID Clang)
find_program_default(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_ID Clang)

set(CMAKE_CXX_COMPILER_TARGET x86_64-pc-windows-msvc)
set(CMAKE_C_COMPILER_TARGET   x86_64-pc-windows-msvc)
set(CMAKE_LINKER              lld-link)

if(NOT CMAKE_GENERATOR MATCHES "Visual Studio")
	set(CMAKE_C_FLAGS_INIT -m64)
	set(CMAKE_CXX_FLAGS_INIT -m64)
	set(ENV{LDFLAGS} "-fuse-ld=lld-link")
endif()

find_program(ccache_program ccache)
if (NOT ${ccache_program} MATCHES "NOTFOUND")
	set(CMAKE_C_COMPILER_LAUNCHER ccache)
	set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif()


# set(CMAKE_C_COMPILER ${CMAKE_VS_ROOT}/VC/Tools/Llvm/bin/clang-cl.exe)
# set(CMAKE_CXX_COMPILER ${CMAKE_VS_ROOT}/VC/Tools/Llvm/bin/clang-cl.exe)

# list(APPEND CMAKE_PROGRAM_PATH ${CMAKE_KITS_DIRECTORY}/bin/${CMAKE_SYSTEM_VERSION}/${ARCH})

