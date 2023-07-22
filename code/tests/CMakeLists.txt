
if(NOT GRM_BUILD_TEST)
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

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${GRM_TEST_DIR}/src/*.cpp")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${GRM_TEST_DIR}/src/*.h")

add_executable(resman_tests ${sources} ${headers})
target_link_libraries(resman_tests PRIVATE
	golxzn::resman
	Catch2::Catch2WithMain
)
set_target_properties(resman_tests PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY ${GRM_ROOT}/bin
)
set_target_properties(tests PROPERTIES FOLDER "golxzn")

include(CTest)
include(Catch)

add_test(NAME Tests COMMAND resman_tests WORKING_DIRECTORY ${GRM_ROOT}/bin)
