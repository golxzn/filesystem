
set(USE_FOLDERS ON)
set(CMAKE_REQUIRED_QUIET ON)

list(APPEND CMAKE_MODULE_PATH
	${CMAKE_SOURCE_DIR}/cmake
	${CMAKE_SOURCE_DIR}/cmake/tools
)

option(GRM_GENERATE_INFO_HEADER "Generate info header" OFF)

include(GetSystemInfo)

get_system_info(GRM_SYSTEM GRM_ARCH)

set(GRM_OUT ${CMAKE_BINARY_DIR})
set(GRM_ROOT ${CMAKE_SOURCE_DIR})
set(GRM_CODE_DIR         ${GRM_ROOT}/code         CACHE PATH "Code directory")
set(GRM_DEPS_DIR         ${GRM_ROOT}/libs         CACHE PATH "Dependencies directory")

# App info
set(GRM_APP_AUTHOR         "Ruslan Golovinskii")

if (NOT CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 20)
endif()

if(CMAKE_CXX_STANDARD LESS 20)
	message(FATAL_ERROR "CMAKE_CXX_STANDARD must be at least 20")
endif()

message(STATUS "-- -- -- -- -- -- -- Configuration -- -- -- -- -- -- -- --")
message(STATUS "System:                 ${GRM_SYSTEM} (${GRM_ARCH})")
message(STATUS "C++ Standard:           ${CMAKE_CXX_STANDARD}")
message(STATUS "C Standard:             ${CMAKE_C_STANDARD}")
message(STATUS "Compiler (ID):          ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_ID})")

message(STATUS "Root directory:         ${GRM_ROOT}")
message(STATUS "Build directory:        ${GRM_OUT}")
message(STATUS "Code directory:         ${GRM_CODE_DIR}")
message(STATUS "Dependencies directory: ${GRM_DEPS_DIR}")
message(STATUS "-- -- -- -- -- -- -- -- -- - -- -- -- -- -- -- -- -- -- --")
