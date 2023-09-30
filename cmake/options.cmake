
set(USE_FOLDERS ON)
set(CMAKE_REQUIRED_QUIET ON)

list(APPEND CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/cmake
	${CMAKE_SOURCE_DIR}/cmake/tools
)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(GRM_IS_TOPLEVEL_PROJECT TRUE)
else()
	set(GRM_IS_TOPLEVEL_PROJECT FALSE)
endif()

option(GRM_BUILD_TEST           "Build filesystem's tests" ${GRM_IS_TOPLEVEL_PROJECT})
option(GRM_DEV_MODE             "Developer mode" ${GRM_IS_TOPLEVEL_PROJECT})
option(GRM_GENERATE_INFO_HEADER "Generate info header" OFF)
option(GRM_GENERATE_DOCS        "Generate MCSS documentation" ${GRM_IS_TOPLEVEL_PROJECT})
mark_as_advanced(GRM_DEV_MODE GRM_GENERATE_INFO_HEADER GRM_GENERATE_DOCS)

include(GetSystemInfo)

get_system_info(GRM_SYSTEM GRM_ARCH)

set(GRM_OUT ${CMAKE_BINARY_DIR})
set(GRM_CODE_DIR ${GRM_ROOT}/code/filesystem CACHE PATH "Code directory")
set(GRM_TEST_DIR ${GRM_ROOT}/code/tests      CACHE PATH "Tests directory")
set(GRM_DOCS_DIR ${GRM_ROOT}/docs            CACHE PATH "Documentation directory")

# App info
set(GRM_APP_AUTHOR         "Ruslan Golovinskii")

if (NOT CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 20)
endif()

if(CMAKE_CXX_STANDARD LESS 20)
	message(FATAL_ERROR "CMAKE_CXX_STANDARD must be at least 20")
endif()

message(STATUS "-- -- -- -- -- -- -- filesystem configuration -- -- -- -- -- -- -- --")
message(STATUS "System:                 ${GRM_SYSTEM} (${GRM_ARCH})")
message(STATUS "C++ Standard:           ${CMAKE_CXX_STANDARD}")
message(STATUS "C Standard:             ${CMAKE_C_STANDARD}")
message(STATUS "Compiler (ID):          ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_ID})")
message(STATUS "Root directory:         ${GRM_ROOT}")
message(STATUS "Build directory:        ${GRM_OUT}")
message(STATUS "Code directory:         ${GRM_CODE_DIR}")
message(STATUS "Top level:              ${GRM_IS_TOPLEVEL_PROJECT}")

if(GRM_DEV_MODE)
	message(STATUS "Tests:                  ${GRM_BUILD_TEST}")
	message(STATUS "Generate info header:   ${GRM_GENERATE_INFO_HEADER}")
	message(STATUS "Generate documentation: ${GRM_GENERATE_DOCS}")
	message(STATUS "Documentation directory:${GRM_DOCS_DIR}")
endif()

message(STATUS "-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --")
