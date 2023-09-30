
if(NOT GXZN_OS_FS_BUILD_TEST)
	return()
endif()

include(FetchContent)

set(BUILD_SHARED_LIBS OFF)

message(CHECK_START "Fetching Catch2...")
FetchContent_Declare(Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v3.3.2
)

FetchContent_MakeAvailable(Catch2)
message(CHECK_PASS "Catch2 fetched")

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${GXZN_OS_FS_TEST_DIR}/src/*.cpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${GXZN_OS_FS_TEST_DIR}/src/*.h")

add_executable(filesystem_tests ${sources} ${headers})
target_link_libraries(filesystem_tests PRIVATE
	golxzn::os::filesystem
	Catch2::Catch2WithMain
)
set_target_properties(filesystem_tests PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${GXZN_OS_FS_ROOT}/bin
	FOLDER "golxzn"
)

include(CTest)
include(Catch)

add_test(NAME Tests COMMAND filesystem_tests WORKING_DIRECTORY ${GXZN_OS_FS_ROOT}/bin)
