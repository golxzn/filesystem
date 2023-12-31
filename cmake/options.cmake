
set(USE_FOLDERS ON)
set(CMAKE_REQUIRED_QUIET ON)

list(APPEND CMAKE_MODULE_PATH
	${GXZN_OS_FS_ROOT}/cmake
	${GXZN_OS_FS_ROOT}/cmake/tools
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(GXZN_OS_FS_IS_TOPLEVEL_PROJECT TRUE)
else()
	set(GXZN_OS_FS_IS_TOPLEVEL_PROJECT FALSE)
endif()

option(GXZN_OS_FS_SHOW_SUBMODULE_INFO  "Show submodule info"         ${GXZN_OS_FS_IS_TOPLEVEL_PROJECT})
option(GXZN_OS_FS_BUILD_TEST           "Build filesystem's tests"    ${GXZN_OS_FS_IS_TOPLEVEL_PROJECT})
option(GXZN_OS_FS_DEV_MODE             "Developer mode"              ${GXZN_OS_FS_IS_TOPLEVEL_PROJECT})
option(GXZN_OS_FS_GENERATE_DOCS        "Generate MCSS documentation" ${GXZN_OS_FS_IS_TOPLEVEL_PROJECT})
option(GXZN_OS_FS_GENERATE_INFO_HEADER "Generate info header"        OFF)
mark_as_advanced(GXZN_OS_FS_DEV_MODE GXZN_OS_FS_GENERATE_INFO_HEADER GXZN_OS_FS_GENERATE_DOCS)

include(GetSystemInfo)

get_system_info(GXZN_OS_FS_SYSTEM GXZN_OS_FS_ARCH)

set(GXZN_OS_FS_OUT ${CMAKE_BINARY_DIR})
set(GXZN_OS_FS_CODE_DIR ${GXZN_OS_FS_ROOT}/code/filesystem CACHE PATH "Code directory")
set(GXZN_OS_FS_TEST_DIR ${GXZN_OS_FS_ROOT}/code/tests      CACHE PATH "Tests directory")
set(GXZN_OS_FS_DOCS_DIR ${GXZN_OS_FS_ROOT}/docs            CACHE PATH "Documentation directory")
set(GXZN_OS_FS_DOCS_PROJECT_NAME "📂 golxzn::os::filesystem 📂")

# App info
set(GXZN_OS_FS_APP_AUTHOR         "Ruslan Golovinskii")

if (NOT CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_CXX_STANDARD LESS 17)
	message(FATAL_ERROR "CMAKE_CXX_STANDARD must be at least 17")
endif()

find_program(ccache_program ccache)
if (NOT ${ccache_program} MATCHES "NOTFOUND")
	set(CMAKE_C_COMPILER_LAUNCHER ccache)
	set(CMAKE_CXX_COMPILER_LAUNCHER ccache)
endif()

if (GXZN_OS_FS_SHOW_SUBMODULE_INFO)

message(STATUS "-- -- -- -- -- -- -- filesystem configuration -- -- -- -- -- -- -- --")
message(STATUS "System:                 ${GXZN_OS_FS_SYSTEM} (${GXZN_OS_FS_ARCH})")
message(STATUS "C++ Standard:           ${CMAKE_CXX_STANDARD}")
message(STATUS "C Standard:             ${CMAKE_C_STANDARD}")
message(STATUS "Root directory:         ${GXZN_OS_FS_ROOT}")
message(STATUS "Build directory:        ${GXZN_OS_FS_OUT}")
message(STATUS "Code directory:         ${GXZN_OS_FS_CODE_DIR}")
message(STATUS "Top level:              ${GXZN_OS_FS_IS_TOPLEVEL_PROJECT}")

if(GXZN_OS_FS_DEV_MODE)
	message(STATUS "Tests:                  ${GXZN_OS_FS_BUILD_TEST}")
	message(STATUS "Generate info header:   ${GXZN_OS_FS_GENERATE_INFO_HEADER}")
	message(STATUS "Generate documentation: ${GXZN_OS_FS_GENERATE_DOCS}")
	message(STATUS "Documentation directory:${GXZN_OS_FS_DOCS_DIR}")
endif()

message(STATUS "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --")

endif()
